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

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

#include <map>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::vector;

TH1D* copy_TH( const TH1D* t_original );
TH2D* copy_TH( const TH2D* t_original );
TH3D* copy_TH( const TH3D* t_original );

vector< void* > export_TH( const TH1D* t_original );
vector< void* > export_TH( const TH2D* t_original );
vector< void* > export_TH( const TH3D* t_original );

void import_TH( vector< void* >& t_exported, TH1D* t_copy );
void import_TH( vector< void* >& t_exported, TH2D* t_copy );
void import_TH( vector< void* >& t_exported, TH3D* t_copy );

bool check_copy( const TH1D* t_original, TH1D* t_copy );
bool check_copy( const TH2D* t_original, TH2D* t_copy );
bool check_copy( const TH3D* t_original, TH3D* t_copy );

template< typename type_ID, typename type_hist >
class THistMap : map< type_ID, type_hist* > {
    public:
        THistMap();
        THistMap( const THistMap& t_THistMap );

        ~THistMap();

        void operator=( const THistMap& t_THistMap );
};

template< typename type_ID, typename type_hist >
class THistReader {
    public:
        THistReader( const vector< string >& t_hists_paths, const vector< type_ID >& t_hists_IDs,
                     const vector< string >& t_hists_names );

        THistReader( const THistReader& t_THistReader );

        ~THistReader();

        type_hist*                      get_hist( const type_ID& t_ID ) const;
        THistMap< type_ID, type_hist >* get_histsMap() const;

        void operator=( const THistReader& t_THistReader );

    private:
        THistMap< type_ID, type_hist > m_hists;
};

#endif