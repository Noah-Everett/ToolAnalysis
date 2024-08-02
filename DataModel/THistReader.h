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

template< typename type_ID, typename type_hist >
class THistMap : public map< type_ID, type_hist* > {
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

TH1D* copy_TH( const TH1D* t_original ) {
    if( ! t_original ) {
        cout << "Error: Original histogram is null" << endl;
        return nullptr;
    }

    vector< void* > exported = export_TH( t_original );
    if( exported.empty() ) {
        cout << "Error: Could not export histogram" << endl;
        return nullptr;
    }

    TH1D* t_copy = nullptr;
    import_TH( exported, t_copy );
    if( ! t_copy ) {
        cout << "Error: Could not copy histogram" << endl;
        return nullptr;
    }

    return t_copy;
}

TH2D* copy_TH( const TH2D* t_original ) {
    if( ! t_original ) {
        cout << "Error: Original histogram is null" << endl;
        return nullptr;
    }

    vector< void* > exported = export_TH( t_original );
    if( exported.empty() ) {
        cout << "Error: Could not export histogram" << endl;
        return nullptr;
    }

    TH2D* t_copy = nullptr;
    import_TH( exported, t_copy );
    if( ! t_copy ) {
        cout << "Error: Could not copy histogram" << endl;
        return nullptr;
    }

    return t_copy;
}

TH3D* copy_TH( const TH3D* t_original ) {
    if( ! t_original ) {
        cout << "Error: Original histogram is null" << endl;
        return nullptr;
    }

    vector< void* > exported = export_TH( t_original );
    if( exported.empty() ) {
        cout << "Error: Could not export histogram" << endl;
        return nullptr;
    }

    TH3D* t_copy = nullptr;
    import_TH( exported, t_copy );
    if( ! t_copy ) {
        cout << "Error: Could not copy histogram" << endl;
        return nullptr;
    }

    return t_copy;
}

vector< void* > export_TH( const TH1D* t_original ) {
    if( ! t_original ) {
        cout << "Error: Original histogram is null" << endl;
        return {};
    }

    vector< void* > exported;
    exported.push_back( ( void* )( new TString( t_original->GetName() ) ) );
    exported.push_back( ( void* )( new TString( t_original->GetTitle() ) ) );
    exported.push_back( ( void* )( new Int_t( t_original->GetNbinsX() ) ) );
    exported.push_back( ( void* )( new Double_t( t_original->GetXaxis()->GetXmin() ) ) );
    exported.push_back( ( void* )( new Double_t( t_original->GetXaxis()->GetXmax() ) ) );

    for( Int_t i = 1; i <= t_original->GetNbinsX(); ++i ) {
        exported.push_back( ( void* )( new Double_t( t_original->GetBinContent( i ) ) ) );
        exported.push_back( ( void* )( new Double_t( t_original->GetBinError( i ) ) ) );
    }

    return exported;
}

vector< void* > export_TH( const TH2D* t_original ) {
    if( ! t_original ) {
        cout << "Error: Original histogram is null" << endl;
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

    for( Int_t i = 1; i <= t_original->GetNbinsX(); ++i ) {
        for( Int_t j = 1; j <= t_original->GetNbinsY(); ++j ) {
            exported.push_back( ( void* )( new Double_t( t_original->GetBinContent( i, j ) ) ) );
            exported.push_back( ( void* )( new Double_t( t_original->GetBinError( i, j ) ) ) );
        }
    }

    return exported;
}

vector< void* > export_TH( const TH3D* t_original ) {
    if( ! t_original ) {
        cout << "Error: Original histogram is null" << endl;
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

void import_TH( vector< void* >& t_exported, TH1D* t_copy ) {
    if( t_exported.empty() ) {
        cout << "Error: Exported histogram is empty" << endl;
        return;
    }

    if( t_copy ) delete t_copy;

    TString*  name   = static_cast< TString* >( t_exported[ 0 ] );
    TString*  title  = static_cast< TString* >( t_exported[ 1 ] );
    Int_t*    nxbins = static_cast< Int_t* >( t_exported[ 2 ] );
    Double_t* xlow   = static_cast< Double_t* >( t_exported[ 3 ] );
    Double_t* xhigh  = static_cast< Double_t* >( t_exported[ 4 ] );

    Int_t nxbins_val = *nxbins;

    if( t_exported.size() != 5 + 2 * nxbins_val ) {
        cout << "Error: Exported histogram does not match expected size" << endl;
        goto cleanup;
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
        if( content ) delete ( Double_t* ) content;
    }

    return;
}

void import_TH( vector< void* >& t_exported, TH2D* t_copy ) {
    if( t_exported.empty() ) {
        cout << "Error: Exported histogram is empty" << endl;
        return;
    }

    if( t_copy ) delete t_copy;

    TString*  name   = static_cast< TString* >( t_exported[ 0 ] );
    TString*  title  = static_cast< TString* >( t_exported[ 1 ] );
    Int_t*    nxbins = static_cast< Int_t* >( t_exported[ 2 ] );
    Double_t* xlow   = static_cast< Double_t* >( t_exported[ 3 ] );
    Double_t* xhigh  = static_cast< Double_t* >( t_exported[ 4 ] );
    Int_t*    nybins = static_cast< Int_t* >( t_exported[ 5 ] );
    Double_t* ylow   = static_cast< Double_t* >( t_exported[ 6 ] );
    Double_t* yhigh  = static_cast< Double_t* >( t_exported[ 7 ] );

    Int_t nxbins_val = *nxbins;
    Int_t nybins_val = *nybins;

    if( t_exported.size() != 8 + 2 * nxbins_val * nybins_val ) {
        cout << "Error: Exported histogram does not match expected size" << endl;
        goto cleanup;
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
        if( content ) delete ( Double_t* ) content;
    }
}

void import_TH( vector< void* >& t_exported, TH3D* t_copy ) {
    if( t_exported.empty() ) {
        cout << "Error: Exported histogram is empty" << endl;
        return;
    }

    if( t_copy ) delete t_copy;

    TString*  name   = static_cast< TString* >( t_exported[ 0 ] );
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

    if( t_exported.size() != 11 + 2 * nxbins_val * nybins_val * nzbins_val ) {
        cout << "Error: Exported histogram does not match expected size" << endl;
        goto cleanup;
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
        if( content ) delete ( Double_t* ) content;
    }

    return;
}

bool check_copy( const TH1D* t_original, const TH1D* t_copy ) {
    if( ! t_original || ! t_copy ) {
        cout << "Error: Original or copied histogram is null" << endl;
        return false;
    }

    if( t_original->GetName() != t_copy->GetName() ) {
        cout << "Error: Copied histogram name does not match original" << endl;
        return false;
    } else if( t_original->GetTitle() != t_copy->GetTitle() ) {
        cout << "Error: Copied histogram title does not match original" << endl;
        return false;
    } else if( t_original->GetNbinsX() != t_copy->GetNbinsX() ) {
        cout << "Error: Copied histogram number of bins does not match original" << endl;
        return false;
    } else if( t_original->GetXaxis()->GetXmin() != t_copy->GetXaxis()->GetXmin() ) {
        cout << "Error: Copied histogram x-axis minimum does not match original" << endl;
        return false;
    } else if( t_original->GetXaxis()->GetXmax() != t_copy->GetXaxis()->GetXmax() ) {
        cout << "Error: Copied histogram x-axis maximum does not match original" << endl;
        return false;
    }

    for( Int_t i = 1; i <= t_original->GetNbinsX(); ++i ) {
        if( t_original->GetBinContent( i ) != t_copy->GetBinContent( i ) ) {
            cout << "Error: Copied histogram bin content does not match original" << endl;
            return false;
        } else if( t_original->GetBinError( i ) != t_copy->GetBinError( i ) ) {
            cout << "Error: Copied histogram bin error does not match original" << endl;
            return false;
        }
    }

    return true;
}

bool check_copy( const TH2D* t_original, const TH2D* t_copy ) {
    if( ! t_original || ! t_copy ) {
        cout << "Error: Original or copied histogram is null" << endl;
        return false;
    }

    if( t_original->GetName() != t_copy->GetName() ) {
        cout << "Error: Copied histogram name does not match original" << endl;
        return false;
    } else if( t_original->GetTitle() != t_copy->GetTitle() ) {
        cout << "Error: Copied histogram title does not match original" << endl;
        return false;
    } else if( t_original->GetNbinsX() != t_copy->GetNbinsX() ) {
        cout << "Error: Copied histogram number of x bins does not match original" << endl;
        return false;
    } else if( t_original->GetXaxis()->GetXmin() != t_copy->GetXaxis()->GetXmin() ) {
        cout << "Error: Copied histogram x-axis minimum does not match original" << endl;
        return false;
    } else if( t_original->GetXaxis()->GetXmax() != t_copy->GetXaxis()->GetXmax() ) {
        cout << "Error: Copied histogram x-axis maximum does not match original" << endl;
        return false;
    } else if( t_original->GetNbinsY() != t_copy->GetNbinsY() ) {
        cout << "Error: Copied histogram number of y bins does not match original" << endl;
        return false;
    } else if( t_original->GetYaxis()->GetXmin() != t_copy->GetYaxis()->GetXmin() ) {
        cout << "Error: Copied histogram y-axis minimum does not match original" << endl;
        return false;
    } else if( t_original->GetYaxis()->GetXmax() != t_copy->GetYaxis()->GetXmax() ) {
        cout << "Error: Copied histogram y-axis maximum does not match original" << endl;
        return false;
    }

    for( Int_t i = 1; i <= t_original->GetNbinsX(); ++i ) {
        for( Int_t j = 1; j <= t_original->GetNbinsY(); ++j ) {
            if( t_original->GetBinContent( i, j ) != t_copy
                ->GetBinContent( i, j ) ) {
                cout << "Error: Copied histogram bin content does not match original" << endl;
                return false;
            } else if( t_original->GetBinError( i, j ) != t_copy
                ->GetBinError( i, j ) ) {
                cout << "Error: Copied histogram bin error does not match original" << endl;
                return false;
            }
        }
    }

    return true;
}

bool check_copy( const TH3D* t_original, const TH3D* t_copy ) {
    if( ! t_original || ! t_copy ) {
        cout << "Error: Original or copied histogram is null" << endl;
        return false;
    }

    if( t_original->GetName() != t_copy->GetName() ) {
        cout << "Error: Copied histogram name does not match original" << endl;
        return false;
    } else if( t_original->GetTitle() != t_copy->GetTitle() ) {
        cout << "Error: Copied histogram title does not match original" << endl;
        return false;
    } else if( t_original->GetNbinsX() != t_copy->GetNbinsX() ) {
        cout << "Error: Copied histogram number of x bins does not match original" << endl;
        return false;
    } else if( t_original->GetXaxis()->GetXmin() != t_copy->GetXaxis()->GetXmin() ) {
        cout << "Error: Copied histogram x-axis minimum does not match original" << endl;
        return false;
    } else if( t_original->GetXaxis()->GetXmax() != t_copy->GetXaxis()->GetXmax() ) {
        cout << "Error: Copied histogram x-axis maximum does not match original" << endl;
        return false;
    } else if( t_original->GetNbinsY() != t_copy->GetNbinsY() ) {
        cout << "Error: Copied histogram number of y bins does not match original" << endl;
        return false;
    } else if( t_original->GetYaxis()->GetXmin() != t_copy->GetYaxis()->GetXmin() ) {
        cout << "Error: Copied histogram y-axis minimum does not match original" << endl;
        return false;
    } else if( t_original->GetYaxis()->GetXmax() != t_copy->GetYaxis()->GetXmax() ) {
        cout << "Error: Copied histogram y-axis maximum does not match original" << endl;
        return false;
    } else if( t_original->GetNbinsZ() != t_copy->GetNbinsZ() ) {
        cout << "Error: Copied histogram number of z bins does not match original" << endl;
        return false;
    } else if( t_original->GetZaxis()->GetXmin() != t_copy->GetZaxis()->GetXmin() ) {
        cout << "Error: Copied histogram z-axis minimum does not match original" << endl;
        return false;
    } else if( t_original->GetZaxis()->GetXmax() != t_copy->GetZaxis()->GetXmax() ) {
        cout << "Error: Copied histogram z-axis maximum does not match original" << endl;
        return false;
    }

    for( Int_t i = 1; i <= t_original->GetNbinsX(); ++i ) {
        for( Int_t j = 1; j <= t_original->GetNbinsY(); ++j ) {
            for( Int_t k = 1; k <= t_original->GetNbinsZ(); ++k ) {
                if( t_original->GetBinContent( i, j, k ) != t_copy
                    ->GetBinContent( i, j, k ) ) {
                    cout << "Error: Copied histogram bin content does not match original" << endl;
                    return false;
                } else if( t_original->GetBinError( i, j, k ) != t_copy
                    ->GetBinError( i, j, k ) ) {
                    cout << "Error: Copied histogram bin error does not match original" << endl;
                    return false;
                }
            }
        }
    }

    return true;
}

template< typename type_ID, typename type_hist >
THistMap< type_ID, type_hist >::THistMap() {
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
void THistMap< type_ID, type_hist >::operator=( const THistMap& t_THistMap ) {
    for( pair< type_ID, type_hist* > entry : t_THistMap ) {
        if( ! entry.second ) {
            cout << "Error: Histogram is null" << endl;
            continue;
        }
        type_hist* temp = copy_TH( entry.second );
        if( ! temp ) {
            cout << "Error: Could not copy histogram" << endl;
            continue;
        } else if( ! check_copy( entry.second, temp ) ) {
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
                                                const vector< string >& t_hists_names ) {
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

        vector< void* > exported { export_TH( temp ) };
        if( exported.empty() ) {
            cout << "Error: Could not export histogram" << endl;
            delete temp;
            continue;
        }
        type_hist* temp2{ nullptr };
        import_TH( exported, temp2 );
        if( ! check_copy( temp, temp2 ) ) {
            cout << "Error: Copied histogram does not match original" << endl;
            delete temp;
            delete temp2;
            continue;
        }
        delete temp2;

        file.Close();

        import_TH( exported, entry.second );
        if( ! entry.second ) {
            cout << "Error: Could not import histogram" << endl;
            delete temp;
            continue;
        }

        auto result = m_hists.insert( entry );
        if( ! result.second ) {
            cout << "Error: Could not insert histogram into map" << endl;
        }

        delete temp;
    }

    if( m_hists.size() != t_hists_paths.size() ) {
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
THistMap< type_ID, type_hist >* THistReader< type_ID, type_hist >::get_histsMap() const {
    return new THistMap< type_ID, type_hist >( m_hists );
}

template< typename type_ID, typename type_hist >
void THistReader< type_ID, type_hist >::operator=( const THistReader& t_THistReader ) {
    if( ! t_THistReader.get_histsMap() ) {
        cout << "Error: THistReader histogram map is null" << endl;
        return;
    }

    m_hists = *( t_THistReader.get_histsMap() );

    if( m_hists->size() != t_THistReader.get_histsMap()->size() ) {
        cout << "Error: Not all histograms were copied" << endl;
    }

    return;
}

#endif