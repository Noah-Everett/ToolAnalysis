// DetectorResponsePrediction.h
///////////////////////////////////////////
// Generalized detector response prediction.
// All information (PDFs, simulations, etc.)
// needs to be set by the user.
//
// Notation: t_ = function variable
//           m_ = member variable
//           naming: mainVariable_subCategory (use '_' like LaTeX)

#ifndef DETECTORRESPONSEPREDICTION_H
#define DETECTORRESPONSEPREDICTION_H

#include <vector>
#include <string>

#include "TH1.h"
#include "TH2.h"

#include "VertexGeometry.h"
#include "THistReader.h"
#include "Particle.h"

#define cout_debug cout<<__FILE__<<"::"<<__FUNCTION__<<" ("<<__LINE__<<"): "

using std::vector;
using std::string;

class DetectorResponsePredictor
{
public:
  /**////////////////////////
  /**/// (Con/De)structor ///
  /**////////////////////////
  /**/
  /**/ DetectorResponsePredictor() {};
  /**/~DetectorResponsePredictor() {};
  /**/
  /**////////////////////////
  
  
  
  /**/////////////
  /**/// Clear ///
  /**/////////////
  /**/
  /**/ void reset_members();
  /**/
  /**/////////////



  /**/////////////////
  /**/// Verbosity ///
  /**/////////////////
  /**/
  /**/ void set_verbosity( int t_verbosity ) { m_verbosity = t_verbosity; }
  /**/ int  get_verbosity() { return m_verbosity; }
  /**/
  /**/////////////////

  
  
  /**///////////////////
  /**/// Data Models ///
  /**///////////////////
  /**/
  /**/// Vertex Geometry
  /**/ inline void            set_vtxGeo( VertexGeometry* t_vtxGeo ) { m_vtxGeo = t_vtxGeo; }
  /**/ inline VertexGeometry* get_vtxGeo() { return m_vtxGeo; }
  /**/
  /**///////////////////



  /**/////////////////////////////////
  /**/// Load Physical Information ///
  /**/////////////////////////////////
  /**/
  /**/// Photon Emission
  /**/ void load_hists_emission_mu_water( const vector< string >& t_hists_paths, 
  /**/                                    const vector< int    >& t_hists_IDs  ,
  /**/                                    const vector< string >& t_hists_names );
  /**/ void load_hists_emission_e_water ( const vector< string >& t_hists_paths, 
  /**/                                    const vector< int    >& t_hists_IDs  ,
  /**/                                    const vector< string >& t_hists_names );
  /**/
  /**/// Photon transmission
  /**/ double load_transmission_water( const string& t_hist_path );
  /**/
  /**/// Energy loss
  /**/ void load_hist_dEdX_mu_water( const string& t_hist_path );
  /**/ void load_hist_dEdX_e_water ( const string& t_hist_path );
  /**/
  /**/////////////////////////////////
  
  
  
  /**/////////////////////////////////
  /**/// Load Physical Information ///
  /**/////////////////////////////////
  /**/
  /**/// Photon Emission
  /**/ double get_emission_mu_water( const double& t_trackLength, 
  /**/                               const double& t_photonAngle );
  /**/ double get_emission_e_water ( const double& t_trackLength, 
  /**/                               const double& t_photonAngle );
  /**/
  /**/// Photon transmission
  /**/ double get_transmission_water( const double& t_energy );
  /**/
  /**/// Energy loss
  /**/ double get_dEdX_mu_water( const double& t_energy );
  /**/ double get_dEdX_e_water ( const double& t_energy );
  /**/
  /**/////////////////////////////////
  
  


  /**////////////////////////////////////
  /**/// Detector Response Prediction ///
  /**////////////////////////////////////
  /**/
  /**/ double get_expected_height( Particle* t_particle, RecoDigit* t_digit );
  /**/ double get_expected_time  ( Particle* t_particle, RecoDigit* t_digit );
  /**/
  /**////////////////////////////////////



private:
    /**////////////////
    /**/// Settings ///
    /**////////////////
    /**/
    /**/ int m_verbosity{ 1 }; // 0 = none, 5 = debug
    /**/
    /**////////////////



    /**///////////////////
    /**/// Data Models ///
    /**///////////////////
    /**/
    /**/ VertexGeometry          * m_vtxGeo         { nullptr };
    /**/ THistReader< int, TH2D >* m_histReader_TH2D{ nullptr };
    /**/ THistReader< int, TH1D >* m_histReader_TH1D{ nullptr };
    /**/
    /**///////////////////



    /**////////////////////////////
    /**/// Physical Information ///
    /**////////////////////////////
    /**/
    /**/ map< int, TH2D* >* m_hists_emission_mu{ nullptr };
    /**/ map< int, TH2D* >* m_hists_emission_e { nullptr };
    /**/ map< int, TH2D* >* m_hists_emission_mu_sci  { nullptr };
    /**/ map< int, TH2D* >* m_hists_emission_e_sci   { nullptr };
    /**/ map< int, TH1D* >* m_hists_detRespPDF_PMT   { nullptr };
    /**/ map< int, TH1D* >* m_hists_detRespPDF_LAPPD { nullptr };
    /**/ map< int, TH1D* >* m_hists_detRespPDF_MRD   { nullptr };
    /**/ TH1D             * m_hist_dEdX_mu_tankWall  { nullptr };
    /**/ TH1D             * m_hist_dEdX_e_tankWall   { nullptr };
    /**/ TH1D             * m_hist_dEdX_mu_MRDIron   { nullptr };
    /**/ TH1D             * m_hist_dEdX_e_MRDIron    { nullptr };
    /**/ TH1D             * m_hist_trans_tankWater   { nullptr };
    /**/ TH1D             * m_hist_trans_MRDSci      { nullptr };
    /**/
    /**////////////////////////////
};

#endif