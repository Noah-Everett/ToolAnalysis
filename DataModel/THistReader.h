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

#include "TROOT.h"
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

template< typename type_ID, typename type_hist >
class THistMap : public map< type_ID, type_hist* > {
        THistMap( unsigned int t_verbosity = 1 );
        THistMap( const THistMap& t_THistMap );

        ~THistMap();

        void         set_verbosity( unsigned int t_verbosity );
        unsigned int get_verbosity() const;

        void operator=( const THistMap& t_THistMap );

    private:
        unsigned int         m_verbosity;
        vector< THistMap*& > m_referecnes;
};

template< typename type_ID, typename type_hist >
class THistReader {
    public:
        THistReader( const vector< string >& t_hists_paths, const vector< type_ID >& t_hists_IDs, const vector< string >& t_hists_names,
                     const string& t_hists_class, unsigned int t_verbosity = 1 );

        THistReader( const THistReader& t_THistReader );

        ~THistReader();

        type_hist*                      get_hist( const type_ID& t_ID ) const;
        shared_ptr< THistMap< type_ID, type_hist > > get_histsMap() const;

        void operator=( const THistReader& t_THistReader );

    private:
        shared_ptr< THistMap< type_ID, type_hist > > m_hists{ make_shared< THistMap< type_ID, type_hist > >() };

        unsigned int m_verbosity;

        enum m_verbosity_enum
        {
            m_verbosity_error   = 0,
            m_verbosity_warning = 1,
            m_verbosity_message = 2,
            m_verbosity_debug   = 3
        };
};

inline vector< void* > export_TH( const TH1D* t_original, unsigned int t_verbosity = 0 ) {
    if( t_verbosity >= 3 ) cout << "Exporting TH1D";

    if( ! t_original ) {
        cout << "...\n"
             << "Error: Original histogram is null" << endl;
        return {};
    }

    vector< void* > exported;
    exported.push_back( ( void* )( new TString( t_original->GetName() ) ) );
    exported.push_back( ( void* )( new TString( t_original->GetTitle() ) ) );
    exported.push_back( ( void* )( new Int_t( t_original->GetNbinsX() ) ) );
    exported.push_back( ( void* )( new Double_t( t_original->GetXaxis()->GetXmin() ) ) );
    exported.push_back( ( void* )( new Double_t( t_original->GetXaxis()->GetXmax() ) ) );

    if( t_verbosity >= 3 )
        cout << " with the properties: {"
             << "Name = " << t_original->GetName() << ", "
             << "Title = " << t_original->GetTitle() << ", "
             << "NbinsX = " << t_original->GetNbinsX() << ", "
             << "Xmin = " << t_original->GetXaxis()->GetXmin() << ", "
             << "Xmax = " << t_original->GetXaxis()->GetXmax() << "}" << endl;

    for( Int_t i = 1; i <= t_original->GetNbinsX(); ++i ) {
        exported.push_back( ( void* )( new Double_t( t_original->GetBinContent( i ) ) ) );
        exported.push_back( ( void* )( new Double_t( t_original->GetBinError( i ) ) ) );
    }

    return exported;
}

inline vector< void* > export_TH( const TH2D* t_original, unsigned int t_verbosity = 0 ) {
    if( t_verbosity >= 3 ) cout << "Exporting TH2D";

    if( ! t_original ) {
        cout << "...\n"
             << "Error: Original histogram is null" << endl;
        return {};
    }

    vector< void* > exported;
    exported.push_back( ( void* )( new TString( t_original->GetName() ) ) );
    exported.push_back( ( void* )( new TString( t_original->GetTitle() ) ) );
    exported.push_back( ( void* )( new Int_t( t_original->GetNbinsX() ) ) );
    exported.push_back( ( void* )( new Double_t( t_original->GetXaxis()->GetXmin() ) ) );
    exported.push_back( ( void* )( new Double_t( t_original->GetXaxis()->GetXmax() ) ) );
    exported.push_back( ( void* )( new Int_t( t_original->GetNbinsY() ) ) );
    exported.push_back( ( void* )( new Double_t( t_original->GetYaxis()->GetXmin() ) ) );
    exported.push_back( ( void* )( new Double_t( t_original->GetYaxis()->GetXmax() ) ) );

    if( t_verbosity >= 3 )
        cout << " with the properties: {"
             << "Name = " << t_original->GetName() << ", "
             << "Title = " << t_original->GetTitle() << ", "
             << "NbinsX = " << t_original->GetNbinsX() << ", "
             << "Xmin = " << t_original->GetXaxis()->GetXmin() << ", "
             << "Xmax = " << t_original->GetXaxis()->GetXmax() << ", "
             << "NbinsY = " << t_original->GetNbinsY() << ", "
             << "Ymin = " << t_original->GetYaxis()->GetXmin() << ", "
             << "Ymax = " << t_original->GetYaxis()->GetXmax() << "}" << endl;

    for( Int_t i = 1; i <= t_original->GetNbinsX(); ++i ) {
        for( Int_t j = 1; j <= t_original->GetNbinsY(); ++j ) {
            exported.push_back( ( void* )( new Double_t( t_original->GetBinContent( i, j ) ) ) );
            exported.push_back( ( void* )( new Double_t( t_original->GetBinError( i, j ) ) ) );
        }
    }

    return exported;
}

inline vector< void* > export_TH( const TH3D* t_original, unsigned int t_verbosity = 0 ) {
    if( t_verbosity >= 3 ) cout << "Exporting TH3D";

    if( ! t_original ) {
        cout << "...\n"
             << "Error: Original histogram is null" << endl;
        return {};
    }

    vector< void* > exported;
    exported.push_back( ( void* )( new TString( t_original->GetName() ) ) );
    exported.push_back( ( void* )( new TString( t_original->GetTitle() ) ) );
    exported.push_back( ( void* )( new Int_t( t_original->GetNbinsX() ) ) );
    exported.push_back( ( void* )( new Double_t( t_original->GetXaxis()->GetXmin() ) ) );
    exported.push_back( ( void* )( new Double_t( t_original->GetXaxis()->GetXmax() ) ) );
    exported.push_back( ( void* )( new Int_t( t_original->GetNbinsY() ) ) );
    exported.push_back( ( void* )( new Double_t( t_original->GetYaxis()->GetXmin() ) ) );
    exported.push_back( ( void* )( new Double_t( t_original->GetYaxis()->GetXmax() ) ) );
    exported.push_back( ( void* )( new Int_t( t_original->GetNbinsZ() ) ) );
    exported.push_back( ( void* )( new Double_t( t_original->GetZaxis()->GetXmin() ) ) );
    exported.push_back( ( void* )( new Double_t( t_original->GetZaxis()->GetXmax() ) ) );

    if( t_verbosity >= 3 )
        cout << " with the properties: {"
             << "Name = " << t_original->GetName() << ", "
             << "Title = " << t_original->GetTitle() << ", "
             << "NbinsX = " << t_original->GetNbinsX() << ", "
             << "Xmin = " << t_original->GetXaxis()->GetXmin() << ", "
             << "Xmax = " << t_original->GetXaxis()->GetXmax() << ", "
             << "NbinsY = " << t_original->GetNbinsY() << ", "
             << "Ymin = " << t_original->GetYaxis()->GetXmin() << ", "
             << "Ymax = " << t_original->GetYaxis()->GetXmax() << ", "
             << "NbinsZ = " << t_original->GetNbinsZ() << ", "
             << "Zmin = " << t_original->GetZaxis()->GetXmin() << ", "
             << "Zmax = " << t_original->GetZaxis()->GetXmax() << "}" << endl;

    for( Int_t i = 1; i <= t_original->GetNbinsX(); ++i ) {
        for( Int_t j = 1; j <= t_original->GetNbinsY(); ++j ) {
            for( Int_t k = 1; k <= t_original->GetNbinsZ(); ++k ) {
                exported.push_back( ( void* )( new Double_t( t_original->GetBinContent( i, j, k ) ) ) );
                exported.push_back( ( void* )( new Double_t( t_original->GetBinError( i, j, k ) ) ) );
            }
        }
    }

    return exported;
}

inline void import_TH( vector< void* >& t_exported, TH1D*& t_copy, TString t_name = "", unsigned int t_verbosity = 0 ) {
    if( t_verbosity >= 3 ) cout << "Importing TH1D";

    if( t_exported.empty() ) {
        cout << "...\n"
             << "Error: Exported histogram vector is empty" << endl;
        return;
    }

    if( t_copy ) delete t_copy;

    TString* name{ nullptr };
    if( t_name.EqualTo( "" ) ) {
        name = static_cast< TString* >( t_exported[ 0 ] );
    } else {
        name = new TString( t_name );
    }
    TString*  title  = static_cast< TString* >( t_exported[ 1 ] );
    Int_t*    nxbins = static_cast< Int_t* >( t_exported[ 2 ] );
    Double_t* xlow   = static_cast< Double_t* >( t_exported[ 3 ] );
    Double_t* xhigh  = static_cast< Double_t* >( t_exported[ 4 ] );

    Int_t nxbins_val = *nxbins;

    if( t_verbosity >= 3 ) {
        cout << " with the properties: {";
        if( t_name.EqualTo( "" ) )
            cout << "Name = " << *name << ", ";
        else
            cout << "t_Name = " << t_name << ", " << "Name = " << *name << ", ";
        cout << "Title = " << *title << ", "
             << "NbinsX = " << *nxbins << ", "
             << "Xmin = " << *xlow << ", "
             << "Xmax = " << *xhigh << "}" << endl;
    }

    if( t_exported.size() != 5 + 2 * nxbins_val ) {
        cout << "Error: Exported histogram does not match expected size" << endl;
        goto cleanup;
    }

    // Check if object with the same name already exists
    while( gROOT->FindObjectAny( *name ) ) {
        if( t_verbosity >= 1 )
            cout << "Warning: Object with the name `" << *name << "` already exists. Setting name to `" << *name << "_copy`" << endl;
        *name += "_copy";
    }
    t_copy = new TH1D( *name, *title, *nxbins, *xlow, *xhigh );

    for( Int_t i = 1; i <= nxbins_val; ++i ) {
        Double_t* content = static_cast< Double_t* >( t_exported[ 5 + 2 * ( i - 1 ) ] );
        Double_t* error   = static_cast< Double_t* >( t_exported[ 6 + 2 * ( i - 1 ) ] );

        t_copy->SetBinContent( i, *content );
        t_copy->SetBinError( i, *error );
    }

cleanup:
    delete name;
    delete title;
    delete nxbins;
    delete xlow;
    delete xhigh;
    for( void* content : t_exported ) {
        if( content ) delete( Double_t* )content;
    }

    return;
}

inline void import_TH( vector< void* >& t_exported, TH2D*& t_copy, TString t_name = "", unsigned int t_verbosity = 0 ) {
    if( t_verbosity >= 3 ) cout << "Importing TH2D";

    if( t_exported.empty() ) {
        cout << "...\n"
             << "Error: Exported histogram vector is empty" << endl;
        return;
    }

    if( t_copy ) delete t_copy;

    TString* name{ nullptr };
    if( t_name.EqualTo( "" ) ) {
        name = static_cast< TString* >( t_exported[ 0 ] );
    } else {
        name = new TString( t_name );
    }

    TString*  title  = static_cast< TString* >( t_exported[ 1 ] );
    Int_t*    nxbins = static_cast< Int_t* >( t_exported[ 2 ] );
    Double_t* xlow   = static_cast< Double_t* >( t_exported[ 3 ] );
    Double_t* xhigh  = static_cast< Double_t* >( t_exported[ 4 ] );
    Int_t*    nybins = static_cast< Int_t* >( t_exported[ 5 ] );
    Double_t* ylow   = static_cast< Double_t* >( t_exported[ 6 ] );
    Double_t* yhigh  = static_cast< Double_t* >( t_exported[ 7 ] );

    Int_t nxbins_val = *nxbins;
    Int_t nybins_val = *nybins;

    if( t_verbosity >= 3 ) {
        cout << " with the properties: {";
        if( t_name.EqualTo( "" ) )
            cout << "Name = " << *name << ", ";
        else
            cout << "t_Name = " << t_name << ", " << "Name = " << *name << ", ";
        cout << "Title = " << *title << ", "
             << "NbinsX = " << *nxbins << ", "
             << "Xmin = " << *xlow << ", "
             << "Xmax = " << *xhigh << ", "
             << "NbinsY = " << *nybins << ", "
             << "Ymin = " << *ylow << ", "
             << "Ymax = " << *yhigh << "}" << endl;
    }

    if( t_exported.size() != 8 + 2 * nxbins_val * nybins_val ) {
        cout << "Error: Exported histogram does not match expected size" << endl;
        goto cleanup;
    }

    while( gROOT->FindObjectAny( *name ) ) {
        if( t_verbosity >= 1 )
            cout << "Warning: Object with the name `" << *name << "` already exists. Setting name to `" << *name << "_copy`" << endl;
        *name += "_copy";
    }
    t_copy = new TH2D( *name, *title, nxbins_val, *xlow, *xhigh, nybins_val, *ylow, *yhigh );

    for( Int_t i = 1; i <= nxbins_val; ++i ) {
        for( Int_t j = 1; j <= nybins_val; ++j ) {
            Double_t* content = static_cast< Double_t* >( t_exported[ 8 + 2 * ( i - 1 ) * nybins_val + 2 * ( j - 1 ) ] );
            Double_t* error   = static_cast< Double_t* >( t_exported[ 9 + 2 * ( i - 1 ) * nybins_val + 2 * ( j - 1 ) ] );

            t_copy->SetBinContent( i, j, *content );
            t_copy->SetBinError( i, j, *error );
        }
    }

    return;

cleanup:
    delete name;
    delete title;
    delete nxbins;
    delete xlow;
    delete xhigh;
    delete nybins;
    delete ylow;
    delete yhigh;
    for( void* content : t_exported ) {
        if( content ) delete( Double_t* )content;
    }

    return;
}

inline void import_TH( vector< void* >& t_exported, TH3D*& t_copy, TString t_name = "", unsigned int t_verbosity = 0 ) {
    if( t_verbosity >= 3 ) cout << "Importing TH3D";

    if( t_exported.empty() ) {
        cout << "...\n"
             << "Error: Exported histogram vector is empty" << endl;
        return;
    }

    if( t_copy ) delete t_copy;

    TString* name{ nullptr };
    if( t_name.EqualTo( "" ) ) {
        name = static_cast< TString* >( t_exported[ 0 ] );
    } else {
        name = new TString( t_name );
    }

    TString*  title  = static_cast< TString* >( t_exported[ 1 ] );
    Int_t*    nxbins = static_cast< Int_t* >( t_exported[ 2 ] );
    Double_t* xlow   = static_cast< Double_t* >( t_exported[ 3 ] );
    Double_t* xhigh  = static_cast< Double_t* >( t_exported[ 4 ] );
    Int_t*    nybins = static_cast< Int_t* >( t_exported[ 5 ] );
    Double_t* ylow   = static_cast< Double_t* >( t_exported[ 6 ] );
    Double_t* yhigh  = static_cast< Double_t* >( t_exported[ 7 ] );
    Int_t*    nzbins = static_cast< Int_t* >( t_exported[ 8 ] );
    Double_t* zlow   = static_cast< Double_t* >( t_exported[ 9 ] );
    Double_t* zhigh  = static_cast< Double_t* >( t_exported[ 10 ] );

    Int_t nxbins_val = *nxbins;
    Int_t nybins_val = *nybins;
    Int_t nzbins_val = *nzbins;

    if( t_verbosity >= 3 ) {
        cout << " with the properties: {";
        if( t_name.EqualTo( "" ) )
            cout << "Name = " << *name << ", ";
        else
            cout << "t_Name = " << t_name << ", " << "Name = " << *name << ", ";
        cout << "Title = " << *title << ", "
             << "NbinsX = " << *nxbins << ", "
             << "Xmin = " << *xlow << ", "
             << "Xmax = " << *xhigh << ", "
             << "NbinsY = " << *nybins << ", "
             << "Ymin = " << *ylow << ", "
             << "Ymax = " << *yhigh << ", "
             << "NbinsZ = " << *nzbins << ", "
             << "Zmin = " << *zlow << ", "
             << "Zmax = " << *zhigh << "}" << endl;
    }

    if( t_exported.size() != 11 + 2 * nxbins_val * nybins_val * nzbins_val ) {
        cout << "Error: Exported histogram does not match expected size" << endl;
        goto cleanup;
    }

    while( gROOT->FindObjectAny( *name ) ) {
        if( t_verbosity >= 1 )
            cout << "Warning: Object with the name `" << *name << "` already exists. Setting name to `" << *name << "_copy`" << endl;
        *name += "_copy";
    }
    t_copy = new TH3D( *name, *title, nxbins_val, *xlow, *xhigh, nybins_val, *ylow, *yhigh, nzbins_val, *zlow, *zhigh );

    for( Int_t i = 1; i <= nxbins_val; ++i ) {
        for( Int_t j = 1; j <= nybins_val; ++j ) {
            for( Int_t k = 1; k <= nzbins_val; ++k ) {
                Double_t* content = static_cast< Double_t* >(
                    t_exported[ 11 + 2 * ( i - 1 ) * nybins_val * nzbins_val + 2 * ( j - 1 ) * nzbins_val + 2 * ( k - 1 ) ] );
                Double_t* error = static_cast< Double_t* >(
                    t_exported[ 12 + 2 * ( i - 1 ) * nybins_val * nzbins_val + 2 * ( j - 1 ) * nzbins_val + 2 * ( k - 1 ) ] );

                t_copy->SetBinContent( i, j, k, *content );
                t_copy->SetBinError( i, j, k, *error );
            }
        }
    }

cleanup:
    delete name;
    delete title;
    delete nxbins;
    delete xlow;
    delete xhigh;
    delete nybins;
    delete ylow;
    delete yhigh;
    delete nzbins;
    delete zlow;
    delete zhigh;
    for( void* content : t_exported ) {
        if( content ) delete( Double_t* )content;
    }

    return;
}

inline TH1D* copy_TH( const TH1D* t_original, unsigned int t_verbosity = 0 ) {
    if( t_verbosity >= 3 ) cout << "Copying TH1D...";

    if( ! t_original ) {
        cout << "Error: Original histogram is null" << endl;
        return nullptr;
    }

    vector< void* > exported = export_TH( t_original, t_verbosity );
    if( exported.empty() ) {
        cout << "Error: Could not export histogram" << endl;
        return nullptr;
    }

    TH1D* t_copy = nullptr;
    import_TH( exported, t_copy, "", t_verbosity );
    if( ! t_copy ) {
        cout << "Error: Could not copy histogram" << endl;
        return nullptr;
    }

    if( t_verbosity >= 3 ) cout << "Done copying TH1D" << endl;

    return t_copy;
}

inline TH2D* copy_TH( const TH2D* t_original, unsigned int t_verbosity = 0 ) {
    if( t_verbosity >= 3 ) cout << "Copying TH2D...";

    if( ! t_original ) {
        cout << "Error: Original histogram is null" << endl;
        return nullptr;
    }

    vector< void* > exported = export_TH( t_original, t_verbosity );
    if( exported.empty() ) {
        cout << "Error: Could not export histogram" << endl;
        return nullptr;
    }

    TH2D* t_copy = nullptr;
    import_TH( exported, t_copy, "", t_verbosity );
    if( ! t_copy ) {
        cout << "Error: Could not copy histogram" << endl;
        return nullptr;
    }

    if( t_verbosity >= 3 ) cout << "Done copying TH2D" << endl;

    return t_copy;
}

inline TH3D* copy_TH( const TH3D* t_original, unsigned int t_verbosity = 0 ) {
    if( t_verbosity >= 3 ) cout << "Copying TH3D...";

    if( ! t_original ) {
        cout << "Error: Original histogram is null" << endl;
        return nullptr;
    }

    vector< void* > exported = export_TH( t_original, t_verbosity );
    if( exported.empty() ) {
        cout << "Error: Could not export histogram" << endl;
        return nullptr;
    }

    TH3D* t_copy = nullptr;
    import_TH( exported, t_copy, "", t_verbosity );
    if( ! t_copy ) {
        cout << "Error: Could not copy histogram" << endl;
        return nullptr;
    }

    if( t_verbosity >= 3 ) cout << "Done copying TH3D" << endl;

    return t_copy;
}

template< typename type_hist, typename type_property >
bool check_property( const type_hist* t_0, const type_hist* t_1, const char* ( type_property::*t_property )() const,
                     TString t_property_name, TString t_properties_ignore = "" ) {
    t_property_name.ToLower();
    t_properties_ignore.ToLower();
    if( ! t_properties_ignore.Contains( t_property_name ) && strcmp( ( t_0->*t_property )(), ( t_1->*t_property )() ) ) {
        cout << "Error: Histogram property `" << t_property_name << "` does not match:\n"
             << "  original = " << ( t_0->*t_property )() << "\n"
             << "  copy     = " << ( t_1->*t_property )() << endl;
        return false;
    }

    return true;
}

template< typename type_hist, typename type_property >
bool check_property( const type_hist* t_0, const type_hist* t_1, Int_t ( type_property::*t_property )() const, TString t_property_name,
                     TString t_properties_ignore = "" ) {
    t_property_name.ToLower();
    t_properties_ignore.ToLower();
    if( ! t_properties_ignore.Contains( t_property_name ) && ( t_0->*t_property )() != ( t_1->*t_property )() ) {
        cout << "Error: Histogram property `" << t_property_name << "` does not match:\n"
             << "  original = " << ( t_0->*t_property )() << "\n"
             << "  copy     = " << ( t_1->*t_property )() << endl;
        return false;
    }

    return true;
}

template< typename type_hist, typename type_property >
bool check_property( const type_hist* t_0, const type_hist* t_1, Double_t ( type_property::*t_property )() const, TString t_property_name,
                     TString t_properties_ignore = "", Double_t t_tolerance = 1e-6 ) {
    t_property_name.ToLower();
    t_properties_ignore.ToLower();
    if( ! t_properties_ignore.Contains( t_property_name ) && abs( ( t_0->*t_property )() - ( t_1->*t_property )() ) > t_tolerance ) {
        cout << "Error: Histogram property `" << t_property_name << "` does not match:\n"
             << "  original = " << ( t_0->*t_property )() << "\n"
             << "  copy     = " << ( t_1->*t_property )() << endl;
        return false;
    }

    return true;
}

template< typename type_hist, typename type_property >
bool check_property( const type_hist* t_0, const type_hist* t_1, Double_t ( type_property::*t_property )( Int_t ) const, Int_t t_bin,
                     TString t_property_name, TString t_properties_ignore = "", Double_t t_tolerance = 1e-6 ) {
    t_property_name.ToLower();
    t_properties_ignore.ToLower();
    if( ! t_properties_ignore.Contains( t_property_name ) &&
        abs( ( t_0->*t_property )( t_bin ) - ( t_1->*t_property )( t_bin ) ) > t_tolerance ) {
        cout << "Error: Histogram property `" << t_property_name << "` does not match:\n"
             << "  original = " << ( t_0->*t_property )( t_bin ) << "\n"
             << "  copy     = " << ( t_1->*t_property )( t_bin ) << endl;
        return false;
    }

    return true;
}

template< typename type_hist, typename type_property >
bool check_property( const type_hist* t_0, const type_hist* t_1, Double_t ( type_property::*t_property )( Int_t, Int_t ) const,
                     Int_t t_bin_0, Int_t t_bin_1, TString t_property_name, TString t_properties_ignore = "",
                     Double_t t_tolerance = 1e-6 ) {
    t_property_name.ToLower();
    t_properties_ignore.ToLower();
    if( ! t_properties_ignore.Contains( t_property_name ) &&
        abs( ( t_0->*t_property )( t_bin_0, t_bin_1 ) - ( t_1->*t_property )( t_bin_0, t_bin_1 ) ) > t_tolerance ) {
        cout << "Error: Histogram property `" << t_property_name << "` does not match:\n"
             << "  original = " << ( t_0->*t_property )( t_bin_0, t_bin_1 ) << "\n"
             << "  copy     = " << ( t_1->*t_property )( t_bin_0, t_bin_1 ) << endl;
        return false;
    }

    return true;
}

template< typename type_hist, typename type_property >
bool check_property( const type_hist* t_0, const type_hist* t_1, Double_t ( type_property::*t_property )( Int_t, Int_t, Int_t ) const,
                     Int_t t_bin_0, Int_t t_bin_1, Int_t t_bin_2, TString t_property_name, TString t_properties_ignore = "",
                     Double_t t_tolerance = 1e-6 ) {
    t_property_name.ToLower();
    t_properties_ignore.ToLower();
    if( ! t_properties_ignore.Contains( t_property_name ) &&
        abs( ( t_0->*t_property )( t_bin_0, t_bin_1, t_bin_2 ) - ( t_1->*t_property )( t_bin_0, t_bin_1, t_bin_2 ) ) > t_tolerance ) {
        cout << "Error: Histogram property `" << t_property_name << "` does not match:\n"
             << "  original = " << ( t_0->*t_property )( t_bin_0, t_bin_1, t_bin_2 ) << "\n"
             << "  copy     = " << ( t_1->*t_property )( t_bin_0, t_bin_1, t_bin_2 ) << endl;
        return false;
    }

    return true;
}

template< typename type_hist >
bool check_null( const type_hist* t_0, const type_hist* t_1 ) {
    if( ! t_0 ) {
        cout << "Error: Original histogram is null" << endl;
        return false;
    } else if( ! t_1 ) {
        cout << "Error: Copied histogram is null" << endl;
        return false;
    }

    return true;
}

inline bool check_copy( const TH1D* t_original, const TH1D* t_copy, TString t_properties_ignore = "" ) {
    if( ! check_null( t_original, t_copy ) ) return false;

    bool value = true;

    value &= check_property( t_original, t_copy, &TH1D::GetName, "Name", t_properties_ignore );
    value &= check_property( t_original, t_copy, &TH1D::GetTitle, "Title", t_properties_ignore );
    value &= check_property( t_original, t_copy, &TH1D::GetNbinsX, "NbinsX", t_properties_ignore );

    value &= check_property( t_original->GetXaxis(), t_copy->GetXaxis(), &TAxis::GetXmin, "Xmin", t_properties_ignore );
    value &= check_property( t_original->GetXaxis(), t_copy->GetXaxis(), &TAxis::GetXmax, "Xmax", t_properties_ignore );

    for( Int_t i = 1; i <= t_original->GetNbinsX(); ++i ) {
        if( ! value ) break;
        value &= check_property( t_original, t_copy, &TH1D::GetBinContent, i, "BinContent", t_properties_ignore );
        value &= check_property( t_original, t_copy, &TH1D::GetBinError, i, "BinError", t_properties_ignore );
    }

    return value;
}

inline bool check_copy( const TH2D* t_original, const TH2D* t_copy, TString t_properties_ignore = "" ) {
    if( ! check_null( t_original, t_copy ) ) return false;

    bool value = true;

    value &= check_property( t_original, t_copy, &TH2D::GetName, "Name", t_properties_ignore );
    value &= check_property( t_original, t_copy, &TH2D::GetTitle, "Title", t_properties_ignore );
    value &= check_property( t_original, t_copy, &TH2D::GetNbinsX, "NbinsX", t_properties_ignore );
    value &= check_property( t_original, t_copy, &TH2D::GetNbinsY, "NbinsY", t_properties_ignore );

    value &= check_property( t_original->GetXaxis(), t_copy->GetXaxis(), &TAxis::GetXmin, "Xmin", t_properties_ignore );
    value &= check_property( t_original->GetXaxis(), t_copy->GetXaxis(), &TAxis::GetXmax, "Xmax", t_properties_ignore );
    value &= check_property( t_original->GetYaxis(), t_copy->GetYaxis(), &TAxis::GetXmin, "Ymin", t_properties_ignore );
    value &= check_property( t_original->GetYaxis(), t_copy->GetYaxis(), &TAxis::GetXmax, "Ymax", t_properties_ignore );

    for( Int_t i = 1; i <= t_original->GetNbinsX(); ++i ) {
        for( Int_t j = 1; j <= t_original->GetNbinsY(); ++j ) {
            if( ! value ) break;
            value &= check_property( t_original, t_copy, &TH2D::GetBinContent, i, j, "BinContent", t_properties_ignore );
            value &= check_property( t_original, t_copy, &TH2D::GetBinError, i, j, "BinError", t_properties_ignore );
        }
    }

    return value;
}

inline bool check_copy( const TH3D* t_original, const TH3D* t_copy, TString t_properties_ignore = "" ) {
    if( ! check_null( t_original, t_copy ) ) return false;

    bool value = true;

    value &= check_property( t_original, t_copy, &TH3D::GetName, "Name", t_properties_ignore );
    value &= check_property( t_original, t_copy, &TH3D::GetTitle, "Title", t_properties_ignore );
    value &= check_property( t_original, t_copy, &TH3D::GetNbinsX, "NbinsX", t_properties_ignore );
    value &= check_property( t_original, t_copy, &TH3D::GetNbinsY, "NbinsY", t_properties_ignore );
    value &= check_property( t_original, t_copy, &TH3D::GetNbinsZ, "NbinsZ", t_properties_ignore );

    value &= check_property( t_original->GetXaxis(), t_copy->GetXaxis(), &TAxis::GetXmin, "Xmin", t_properties_ignore );
    value &= check_property( t_original->GetXaxis(), t_copy->GetXaxis(), &TAxis::GetXmax, "Xmax", t_properties_ignore );
    value &= check_property( t_original->GetYaxis(), t_copy->GetYaxis(), &TAxis::GetXmin, "Ymin", t_properties_ignore );
    value &= check_property( t_original->GetYaxis(), t_copy->GetYaxis(), &TAxis::GetXmax, "Ymax", t_properties_ignore );
    value &= check_property( t_original->GetZaxis(), t_copy->GetZaxis(), &TAxis::GetXmin, "Zmin", t_properties_ignore );
    value &= check_property( t_original->GetZaxis(), t_copy->GetZaxis(), &TAxis::GetXmax, "Zmax", t_properties_ignore );

    for( Int_t i = 1; i <= t_original->GetNbinsX(); ++i ) {
        for( Int_t j = 1; j <= t_original->GetNbinsY(); ++j ) {
            for( Int_t k = 1; k <= t_original->GetNbinsZ(); ++k ) {
                if( ! value ) break;
                value &= check_property( t_original, t_copy, &TH3D::GetBinContent, i, j, k, "BinContent", t_properties_ignore );
                value &= check_property( t_original, t_copy, &TH3D::GetBinError, i, j, k, "BinError", t_properties_ignore );
            }
        }
    }

    return value;
}

template< typename type_ID, typename type_hist >
THistMap< type_ID, type_hist >::THistMap( unsigned int t_verbosity )
    : m_verbosity{ t_verbosity } {
}

template< typename type_ID, typename type_hist >
THistMap< type_ID, type_hist >::THistMap( const THistMap& t_THistMap ) {
    *this = t_THistMap;
}

template< typename type_ID, typename type_hist >
THistMap< type_ID, type_hist >::~THistMap() {
    for( pair< type_ID, type_hist* > entry : *this ) {
        if( entry.second ) delete entry.second;
    }
}

template< typename type_ID, typename type_hist >
void THistMap< type_ID, type_hist >::set_verbosity( unsigned int t_verbosity ) {
    m_verbosity = t_verbosity;
}

template< typename type_ID, typename type_hist >
unsigned int THistMap< type_ID, type_hist >::get_verbosity() const {
    return m_verbosity;
}

template< typename type_ID, typename type_hist >
void THistMap< type_ID, type_hist >::operator=( const THistMap& t_THistMap ) {
    m_verbosity = t_THistMap.m_verbosity;

    for( pair< type_ID, type_hist* > entry : t_THistMap ) {
        if( ! entry.second ) {
            cout << "Error: Histogram is null" << endl;
            continue;
        }
        type_hist* temp = copy_TH( entry.second, m_verbosity );
        if( ! temp ) {
            cout << "Error: Could not copy histogram" << endl;
            continue;
        } else if( ! check_copy( entry.second, temp, "NAME" ) ) {
            cout << "Error: Copied histogram does not match original" << endl;
            delete temp;
            continue;
        }

        auto result = this->insert( { entry.first, temp } );
        if( ! result.second ) {
            cout << "Error: Could not insert histogram into map" << endl;
            delete temp;
        }
    }

    if( this->size() != t_THistMap.size() ) {
        cout << "Error: Not all histograms were copied" << endl;
    }
}

template< typename type_ID, typename type_hist >
THistReader< type_ID, type_hist >::THistReader( const vector< string >& t_hists_paths, const vector< type_ID >& t_hists_IDs,
                                                const vector< string >& t_hists_names, const string& t_hists_class,
                                                unsigned int t_verbosity )
    : m_verbosity{ t_verbosity } {
    m_hists->set_verbosity( m_verbosity );

    if( m_verbosity >= m_verbosity_debug ) {
        cout << "THistReader: Loading histograms" << endl;
    }

    if( t_hists_paths.size() != t_hists_IDs.size() || t_hists_paths.size() != t_hists_names.size() ) {
        cout << "Error: hists_paths, hists_IDs, and hists_names "
             << "vectors should all be the same length" << endl;
        return;
    }

    for( int i{ 0 }; i < t_hists_paths.size(); i++ ) {
        TFile file{ t_hists_paths[ i ].c_str() };
        if( file.IsZombie() ) {
            cout << "Error: Could not open file with path " << t_hists_paths[ i ] << endl;
            continue;
        }

        pair< type_ID, type_hist* > entry;
        entry.first = t_hists_IDs[ i ];

        type_hist* temp{ nullptr };
        file.GetObject( t_hists_names[ i ].c_str(), temp );
        if( ! temp ) {
            cout << "Error: Could not find histogram with name " << t_hists_names[ i ] << endl;
            continue;
        }

        vector< void* > exported{ export_TH( temp, m_verbosity ) };
        if( exported.empty() ) {
            cout << "Error: Could not export histogram" << endl;
            delete temp;
            continue;
        }
        type_hist* temp2{ nullptr };
        import_TH( exported, temp2, "TEST", m_verbosity );
        if( ! check_copy( temp, temp2, "NAME" ) ) {
            cout << "Error: Copied histogram does not match original" << endl;
            delete temp;
            delete temp2;
            return;
        }

        delete temp;
        delete temp2;
        file.Close();

        import_TH( exported, entry.second, t_hists_names[ i ] + "_" + to_string( t_hists_IDs[ i ] ) + "MeV_" + t_hists_class, m_verbosity );
        if( ! entry.second ) {
            cout << "Error: Could not import histogram" << endl;
            delete temp;
            return;
        }

        auto result = m_hists->insert( entry );
        if( ! result.second ) {
            cout << "Error: Could not insert histogram into map" << endl;
        }
    }

    if( m_hists->size() != t_hists_paths.size() ) {
        cout << "Error: Not all histograms were loaded" << endl;
    }
}

template< typename type_ID, typename type_hist >
THistReader< type_ID, type_hist >::THistReader( const THistReader& t_THistReader ) {
    *this = t_THistReader;
}

template< typename type_ID, typename type_hist >
THistReader< type_ID, type_hist >::~THistReader() {
}

template< typename type_ID, typename type_hist >
type_hist* THistReader< type_ID, type_hist >::get_hist( const type_ID& t_ID ) const {
    pair< type_ID, type_hist* >* result = m_hists->find( t_ID );
    if( result == m_hists->end() ) {
        cout << "Error: Could not find histogram with ID " << t_ID << endl;
        return nullptr;
    } else if( ! result->second ) {
        cout << "Error: Histogram is null" << endl;
        return nullptr;
    }

    return result->second;
}

template< typename type_ID, typename type_hist >
shared_ptr< THistMap< type_ID, type_hist > > THistReader< type_ID, type_hist >::get_histsMap() const {
    return m_hists;
}

template< typename type_ID, typename type_hist >
void THistReader< type_ID, type_hist >::operator=( const THistReader& t_THistReader ) {
    if( ! t_THistReader.get_histsMap() ) {
        cout << "Error: THistReader histogram map is null" << endl;
        return;
    }

    m_verbosity = t_THistReader.m_verbosity;
    m_hists     = t_THistReader.get_histsMap()

    if( m_hists->size() != t_THistReader.get_histsMap()->size() ) {
        cout << "Error: Not all histograms were copied" << endl;
    }

    return;
}

#endif