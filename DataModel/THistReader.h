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
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

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

    /**//////////////////
    /**/// Copy THist ///
    /**//////////////////
    /**/
    /**/ void copy_THist( const type_hist* t_original, type_hist*& t_copy );
    /**/
    /**//////////////////
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
/**/         if( file.IsZombie() ) {
/**/             cout << "Error: Could not open file with path " << t_hists_paths[ i ] << endl;
/**/             continue;
/**/         }
/**/
/**/         pair< type_ID, type_hist* > entry;
/**/ 	     entry.first = t_hists_IDs[ i ];
/**/
/**/         type_hist* temp{ nullptr };
/**/         file.GetObject( t_hists_names[ i ].c_str(), temp );
/**/         if( !temp ) {
/**/             cout << "Error: Could not find histogram with name " << t_hists_names[ i ] << endl;
/**/             continue;
/**/         }
/**/
/* DELETE */ cout << "temp = " << temp << endl;
/**/         copy_THist( temp, entry.second );
/* DELETE */ cout << "entry.second = " << entry.second << endl;
/**/         if( !entry.second ) {
/**/             cout << "Error: Could not copy histogram" << endl;
/**/             continue;
/**/         }
/**/
/**/         auto result = m_hists->insert( entry );
/**/         if( !result.second )
/**/             cout << "Error: Could not insert histogram into map" << endl;
/**/
/**/         file.Close();
/* DELETE */
/* DELETE */ cout << "DELETE" << endl;
/**/     }
/**/ 
/**/     if( m_hists->size() != t_hists_paths.size() )
/**/         cout << "Error: Not all histograms were loaded" << endl;
/* DELETE */ cout << "Starting to print m_hists" << endl;
/* DELETE */ for( auto it = m_hists->end(); it != m_hists->begin(); ) {
/* DELETE */     --it;
/* DELETE */     cout << "it->first = " << it->first << endl;
/* DELETE */     auto temp = it->second;
/* DELETE */     cout << "temp = " << temp << endl;
/* DELETE */     cout << "temp->GetName() = " << temp->GetName() << endl;
/* DELETE */     cout << "temp->GetTitle() = " << temp->GetTitle() << endl;
/* DELETE */     cout << "temp->GetEntries() = " << temp->GetEntries() << endl;
/* DELETE */ }
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
template<typename type_ID>
void THistReader<type_ID, TH1>::copy_THist(const TH1* t_original, TH1*& t_copy) {
    if (!t_original) {
        cout << "Error: Original histogram is null" << endl;
        return;
    }

    // Create new histogram
    TString name = t_original->GetName();
    TString title = t_original->GetTitle();
    Int_t nxbins = t_original->GetNbinsX();
    Double_t xlow = t_original->GetXaxis()->GetXmin();
    Double_t xhigh = t_original->GetXaxis()->GetXmax();

    t_copy = new TH1(name, title, nxbins, xlow, xhigh);

    // Copy contents
    for (Int_t i = 1; i <= nxbins; ++i) {
        Double_t content = t_original->GetBinContent(i);
        Double_t error = t_original->GetBinError(i);
        t_copy->SetBinContent(i, content);
        t_copy->SetBinError(i, error);
    }
}

template<typename type_ID>
void THistReader<type_ID, TH2>::copy_THist(const TH2* t_original, TH2*& t_copy) {
    if (!t_original) {
        cout << "Error: Original histogram is null" << endl;
        return;
    }

    // Create new histogram
    TString name = t_original->GetName();
    TString title = t_original->GetTitle();
    Int_t nxbins = t_original->GetNbinsX();
    Double_t xlow = t_original->GetXaxis()->GetXmin();
    Double_t xhigh = t_original->GetXaxis()->GetXmax();
    Int_t nybins = t_original->GetNbinsY();
    Double_t ylow = t_original->GetYaxis()->GetXmin();
    Double_t yhigh = t_original->GetYaxis()->GetXmax();

    t_copy = new TH2(name, title, nxbins, xlow, xhigh, nybins, ylow, yhigh);

    // Copy contents
    for (Int_t i = 1; i <= nxbins; ++i) {
        for (Int_t j = 1; j <= nybins; ++j) {
            Double_t content = t_original->GetBinContent(i, j);
            Double_t error = t_original->GetBinError(i, j);
            t_copy->SetBinContent(i, j, content);
            t_copy->SetBinError(i, j, error);
        }
    }
}

template<typename type_ID>
void THistReader<type_ID, TH3>::copy_THist(const TH3* t_original, TH3*& t_copy) {
    if (!t_original) {
        cout << "Error: Original histogram is null" << endl;
        return;
    }

    // Create new histogram
    TString name = t_original->GetName();
    TString title = t_original->GetTitle();
    Int_t nxbins = t_original->GetNbinsX();
    Double_t xlow = t_original->GetXaxis()->GetXmin();
    Double_t xhigh = t_original->GetXaxis()->GetXmax();
    Int_t nybins = t_original->GetNbinsY();
    Double_t ylow = t_original->GetYaxis()->GetXmin();
    Double_t yhigh = t_original->GetYaxis()->GetXmax();
    Int_t nzbins = t_original->GetNbinsZ();
    Double_t zlow = t_original->GetZaxis()->GetXmin();
    Double_t zhigh = t_original->GetZaxis()->GetXmax();

    t_copy = new TH3(name, title, nxbins, xlow, xhigh, nybins, ylow, yhigh, nzbins, zlow, zhigh);

    // Copy contents
    for (Int_t i = 1; i <= nxbins; ++i) {
        for (Int_t j = 1; j <= nybins; ++j) {
            for (Int_t k = 1; k <= nzbins; ++k) {
                Double_t content = t_original->GetBinContent(i, j, k);
                Double_t error = t_original->GetBinError(i, j, k);
                t_copy->SetBinContent(i, j, k, content);
                t_copy->SetBinError(i, j, k, error);
            }
        }
    }
}
/**/
/**////////////////////////////

#endif