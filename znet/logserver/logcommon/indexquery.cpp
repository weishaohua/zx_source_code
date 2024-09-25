#include "indexquery.hpp"
#include "indexdb.hpp"
#include "logcommon/recordreader.hpp"
#include "logcommon/config.hpp"
#include "logcommon/filenamerule.hpp"

namespace LOG
{
	IndexQuery::~IndexQuery()
	{
		for( queryset_t::iterator it = queryset.begin(); it != queryset.end(); ++it )
			delete *it;
	}

	void IndexQuery::add( Query *query ) { queryset.insert(query); }

	void IndexQuery::init()
	{
		logdir = Config2::GetInstance()->GetRecordDir();
                logidxdir = Config2::GetInstance()->GetParseDir();
                logrotatedir = Config2::GetInstance()->GetRotateDir();
	}

	///////////////////////////////////////////////////////////////////////////////////
	//
	// ���û�����������ҵ�,enable_scan_if_not_idxΪ��,˳��ɨ����־ 
	//
	///////////////////////////////////////////////////////////////////////////////////
        void IndexQuery::go()
        {
		init();
		for(queryset_t::iterator it = queryset.begin(); it != queryset.end(); ++it)
		{
			if( !idxquery( *it ) )
			{
				if( (*it)->enable_scan_if_not_found_idx() )
					scan( *it );
			}
		}
        }

	bool IndexQuery::idxquery( Query *query )
        {
		fileset_t idxfile_set;
		std::string logname = get_one_matched_logname( query->get_type() );
                if( ! get_idxfile(query->get_timerange(), query->get_type(), logname, query->get_keyname(), idxfile_set) )
		{
			std::cerr << "get_idxfile false " << std::endl;
			return false;
		}

                IndexInfoSet indexinfo_set;
		get_indexinfo_set( indexinfo_set, idxfile_set, query->get_keyvalue() );
                if( indexinfo_set.empty() )
		{
			std::cerr <<  "no found" << std::endl;
                        return false;
		}
		indexinfo_set.dump();
		search_in_log( indexinfo_set, query );
	        return true;
        }
	
	void IndexQuery::search_in_log( IndexInfoSet &indexinfo_set, Query *query )
	{
		std::string cur_file;
		RecordReader *recordreader;
		for( iiset_t::iterator it_iiset = indexinfo_set.begin(); it_iiset != indexinfo_set.end(); ++it_iiset )
                {
			if( cur_file.empty() )
			{
				cur_file = it_iiset->filename;
				recordreader = new RecordReader( it_iiset->filename, false );
			}
			else if( cur_file != it_iiset->filename )
			{
				delete recordreader;
				recordreader = new RecordReader(it_iiset->filename, false);
				cur_file = it_iiset->filename;
			}
			GNET::Marshal::OctetsStream rec;
			RecordHead rh;
			if( recordreader->seek( *it_iiset  ) &&  recordreader->read(rec, rh, NULL) )
				query->process_protocol( rec );
			else
				printf( "seek error offset = %lld\n", it_iiset->offset );
                }

	}

	//////////////////////////////////////////////////////////////////////////
	//
	// �ֱ��idxfile_set�е��ļ�,
	// ����keyvalueֵ,�ҵ���Ӧ��indexinfo�ڵ���Ϣ,����indexinfo_set��
	//
	//////////////////////////////////////////////////////////////////////////
	void IndexQuery::get_indexinfo_set( IndexInfoSet &indexinfo_set, 
			const fileset_t &idxfile_set, const GNET::Marshal::OctetsStream &keyvalue )
	{
		int i = 1;
		for( fileset_t::iterator it_idxfile = idxfile_set.begin(); it_idxfile != idxfile_set.end(); ++it_idxfile )
		{
			std::cout << "now search idxfile = " << i++ << " = " << *it_idxfile << std::endl;
			IndexInfoSet tmp_set;
			IndexDB indexdb( *it_idxfile );
			indexdb.find( keyvalue, tmp_set );

			if( ! tmp_set.empty() )
			{
				std::string logfile = FilenameRule::idxfile2logfile(*it_idxfile, logdir, logrotatedir );
				std::cout << "catch idx file = " << *it_idxfile << std::endl;
				std::cout << "== > log = " << logfile << std::endl;
				for( iiset_t::iterator it = tmp_set.begin(); it != tmp_set.end(); ++it )
				{
					IndexInfo indexinfo = *it;
					indexinfo.filename = logfile;
					indexinfo_set.insert( indexinfo );
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////
	//
	// ɨ����־
	// �����ǰquery����file_or_dirΪ��, ����get_logfile();
	// �������file_or_dir,��������ļ����쳣
	//
	/////////////////////////////////////////////////////////////////////////
        void IndexQuery::scan( Query *query )
        {
		std::string file_or_dir = query->get_file_or_dir();
		fileset_t logfile_set, tmp_set;
		if( ! file_or_dir.empty() )
		{
			split_str( file_or_dir, FILENAME_SPLIT, tmp_set );
			for( fileset_t::iterator it = tmp_set.begin(); it != tmp_set.end(); ++ it ) 
			{       
				if( FilenameRule::is_file(*it) )
					logfile_set.insert( *it );
				else
					throw LOG::Exception( "please input logname, not dir!" );
			}
		}
		else
			get_logfile( query->get_timerange(), get_one_matched_logname( query->get_type() ), logfile_set );

		for( fileset_t::iterator it = logfile_set.begin(); it != logfile_set.end(); ++it )
		{
			std::cout << "now scan logfile = " << *it << std::endl;
			std::auto_ptr< RecordReader > recordreader( new RecordReader(*it, false) );
			IndexInfo ii;
			RecordHead rh;
			GNET::Marshal::OctetsStream rec;
			while( Env::Inst().running )
                        {
                                if ( ! recordreader->read( rec, rh, &ii ))
                                        break;
				if( query->get_type() == rh.type )
				{
					GNET::Marshal::OctetsStream protocol_data;
					RecordHead recordhead;
					rec >> recordhead >> protocol_data;
					query->process_protocol( protocol_data );
				}
			}
		}
        }

	//////////////////////////////////////////////////////////////////////////////////////
	//
	// ȡ type_file_map �е�����һ����Ϊscan log
	// ���Ϊ��ȡdefault && !deny
	// ���default deny ȡany
	//
	//////////////////////////////////////////////////////////////////////////////////////
	std::string IndexQuery::get_one_matched_logname( const type_t type )
	{
		std::string logname;
		type_file_map_t type_file_map = Config2::GetInstance()->get_type_file_map();
		type_file_map_t::iterator tmit = type_file_map.find( type );
		if( tmit != type_file_map.end() )
		{
			fileset_t::iterator fsit = tmit->second.begin();
			logname =  FilenameRule::get_logdir_logname( *fsit );
		}
		if( logname.empty() )
		{
			std::string anyfile=  Config2::GetInstance()->GetAnyFile();
			if( ! anyfile.empty() )
			{
				logname= FilenameRule::get_logdir_logname( anyfile );
			}
			else
			{
				std::string deflog =  Config2::GetInstance()->GetDefaultFile();
				if (!deflog.empty())
				{
					denyset_t deny = Config2::GetInstance()->GetDenySet();
					if( deny.find( type ) == deny.end() ) //not in deny
						logname = FilenameRule::get_logdir_logname( deflog );
				}
			}
		}
		if( logname.empty() )
			throw LOG::Exception( "no found match type in XML(application) file!" );
		return logname;
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//
	// ȡ��־�ļ�����ת��Ŀ¼��logserver���ڼ�¼��Ŀ¼
	//
	//////////////////////////////////////////////////////////////////////////////////////
	void IndexQuery::get_logfile( const TimeRange &timerange, const std::string &logname, fileset_t &logfile_set ) 
	{
		std::cout << "match logname is: " << logname << std::endl;
		ReadDir readdir( logrotatedir );
		fileset_t tempset = readdir.getfileset();
                fileset_t::const_iterator it = tempset.begin(); 
                for(; it != tempset.end(); ++it)
                {
			std::string fullpath = logrotatedir + *it;
			if( (FilenameRule::get_logrotatedir_logname( fullpath ) == logname )
				&& FilenameRule::check_logrotatedir_file( fullpath )
				&& FilenameRule::match_time_range( fullpath, timerange) 
				)
				logfile_set.insert( fullpath );
		}
		
		ReadDir readlogdir( logdir );
		fileset_t tempset2 = readlogdir.getfileset();
                fileset_t::const_iterator it2 = tempset.begin(); 
                for(; it2 != tempset2.end(); ++it2)
                {
			std::string fullpath = logdir + *it2;
			if( (FilenameRule::get_logdir_logname( fullpath ) == logname ) 
				&& FilenameRule::check_logdir_file( fullpath )
				)
				logfile_set.insert( fullpath );
		}
		
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//
	// ȡ����Ŀ¼�����ļ�
	//
	//////////////////////////////////////////////////////////////////////////////////////
	bool IndexQuery::get_idxfile( const TimeRange &timerange, const type_t &type, const std::string &logname,
					const std::string &keyname, fileset_t &idxfile_set ) 
	{
std::cout << "logidxdir = " << logidxdir << " type " << type << " logname " << logname << " keyname " << keyname << std::endl;
		ReadDir readdir( logidxdir );
		fileset_t tempset = readdir.getfileset();
                fileset_t::const_iterator it = tempset.begin(); 
                for(; it != tempset.end(); ++it)
                {
			std::string fullpath= logidxdir + *it;		
			if( ( FilenameRule::get_logidxdir_logname( fullpath ) == logname )
					&& FilenameRule::check_logidxdir_file( fullpath )
					&& FilenameRule::match_type( fullpath, type) 
					&& FilenameRule::match_keyname( fullpath, keyname )
					&& FilenameRule::match_time_range( fullpath, timerange) )
				idxfile_set.insert( fullpath );
		}
		if( idxfile_set.empty() )
			return false;
		return true;
	}

}// end namespace LOG
