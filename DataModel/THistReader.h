//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                       THistReader.h                                     //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                                                                         //*//
//*// Author:                                                                                 //*//
//*//   Noah Everett (noah.everett@mines.sdsmt.edu)                                           //*//
//*//   Created ¯\_(ツ)_/¯                                                                    //*//
//*//                                                                                         //*//
//*// About:                                                                                  //*//
//*//   This file contains a general class for reading ROOT histograms.                       //*//
//*//                                                                                         //*//
//*// Notation:                                                                               //*//
//*//   t_ = function argument                                                                //*//
//*//   m_ = member variable                                                                  //*//
//*//   mainVariable_subCategory (use '_' like LaTeX)                                         //*//
//*//                                                                                         //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                                                                         //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//

#ifndef THISTREADER_H
#define THISTREADER_H

#include <map>
#include <string>
#include <vector>
#include <ostream>

#include "TFile.h"

using std::map;
using std::string;
using std::vector;
using std::cout;
using std::endl;

template< typename type_ID, typename type_hist >
class THistReader
{
public:
    /**////////////////////////
    /**/// (Con/De)structor ///
    /**////////////////////////
    /**/
    /**/ THistReader( const vector< string  >& t_hists_paths, 
    /**/              const vector< type_ID >& t_hists_IDs  ,
    /**/              const vector< string  >& t_hists_names );
    /**/ THistReader( const THistReader* t_THistReader ): m_hists{ t_THistReader->get_histsMap_cp() } {}
    /**/~THistReader() { delete m_hists; }
    /**/
    /**////////////////////////



    /**////////////////////////
    /**/// Access Functions ///
    /**////////////////////////
    /**/
    /**/ type_hist                 * get_hist       ( const type_ID& t_ID ) const { return m_hists->at( t_ID ); }
    /**/ map< type_ID, type_hist* >* get_histsMap   (                     )       { return m_hists;             }
    /**/ map< type_ID, type_hist* >* get_histsMap_cp(                     ) const;
    /**/
    /**////////////////////////



    /**/////////////////////////
    /**/// Setting Functions ///
    /**/////////////////////////
    /**/
    /**/ void operator=( const THistReader* t_THistReader );
    /**/
    /**/////////////////////////



private:
    /**////////////////////////
    /**/// Member Variables ///
    /**////////////////////////
    /**/
    /**/ map< type_ID, type_hist* >* m_hists{ new map< type_ID, type_hist* > };
    /**/
    /**////////////////////////
};


  
/**////////////////////////////
/**/// Function Definitions ///
/**////////////////////////////
/**/
/**/ template< typename type_ID, typename type_hist >
/**/ THistReader< type_ID, type_hist >::THistReader( const vector< string > & t_hists_paths,
/**/                                                 const vector< type_ID >& t_hists_IDs  ,
/**/                                                 const vector< string  >& t_hists_names ) {
/**/     if( t_hists_paths.size() != t_hists_IDs.size() || t_hists_paths.size() != t_hists_names.size() ) {
/**/         cout << "Error: hists_paths, hists_IDs, and hists_names "
/**/              << "vectors should all be the same length" << endl;
/**/         return;
/**/     }
/**/
/**/     for( int i{ 0 }; i < t_hists_paths.size(); i++ ) {
/**/         TFile file{ t_hists_paths[ i ].c_str() };
/**/         pair< type_ID, type_hist* > entry;
/* DELETE */ cout << "entry.first = " << entry.first << endl;
/* DELETE */ cout << "entry.second = " << entry.second << endl;
/* DELETE */ cout << "t_hists_paths[ i ] = " << t_hists_paths[ i ] << endl;
/* DELETE */ cout << "t_hists_IDs[ i ] = " << t_hists_IDs[ i ] << endl;
/* DELETE */ cout << "t_hists_names[ i ] = " << t_hists_names[ i ] << endl;
/**/ 	     entry.first = t_hists_IDs[ i ];
// /**/         file.GetObject( t_hists_names[ i ].c_str(), entry.second );
// /**/         entry.second = (type_hist*)file.Get( t_hists_names[ i ].c_str() );
/**/         entry.second = new type_hist{ *( (type_hist*)file.Get( t_hists_names[ i ].c_str() ) ) };
/**/         if( !entry.second ) {
/**/             cout << "Error: Could not find histogram with name " << t_hists_names[ i ] << endl;
/**/             return;
/**/         }
/* DELETE */ cout << "entry.first = " << entry.first << endl;
/* DELETE */ cout << "entry.second = " << entry.second << endl;
/**/         m_hists->insert( entry );
/* DELETE */ cout << endl;
/**/     }
/**/ }
/**/
/**/ template< typename type_ID, typename type_hist >
/**/ map< type_ID, type_hist* >* THistReader< type_ID, type_hist >::get_histsMap_cp() const {
/**/     map< type_ID, type_hist* >* temp{ new map< type_ID, type_hist* > };
/**/     *temp = *m_hists;
/**/     return temp;
/**/ }
/**/
/**/
/**/ template< typename type_ID, typename type_hist >
/**/ void THistReader< type_ID, type_hist >::operator=( const THistReader* t_THistReader ) {
/**/     if( m_hists ) delete m_hists;
/**/     m_hists = new map< type_ID, type_hist* >{ *( t_THistReader->m_hists ) };
/**/ }
/**/
/**////////////////////////////

#endif