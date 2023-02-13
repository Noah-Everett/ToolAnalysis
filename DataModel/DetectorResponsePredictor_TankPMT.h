// DetectorResponsePrediction_TankPMT.h
///////////////////////////////////////////
// Generalized detector response prediction.
// All information (PDFs, simulations, etc.)
// needs to be set by the user.
//
// Notation: t_ = function variable
//           m_ = member variable
//           naming: mainVariable_subCategory (use '_' like LaTeX)

#ifndef DETECTORRESPONSEPREDICTION_TANKPMT_H
#define DETECTORRESPONSEPREDICTION_TANKPMT_H

#include <vector>
#include <string>

#include "TH1.h"
#include "TH2.h"

#include "VertexGeometry.h"
#include "THistReader.h"
#include "Particle.h"

#define cout_FFL cout<<__FILE__<<"::"<<__FUNCTION__<<" ("<<__LINE__<<"): "

using std::vector;
using std::string;

class LeptonRecoLhCalc
{
public:
  /**////////////////////////
  /**/// (Con/De)structor ///
  /**////////////////////////
  /**/
  /**/ LeptonRecoLhCalc() {};
  /**/~LeptonRecoLhCalc() {};
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
  /**///////////////////////



  /**////////////////////////////
  /**/// Physical Information ///
  /**////////////////////////////
  /**/
  /**/// Detector response PDFs
  /**/ inline void load_hists_emission_mu_water( const vector< string >& t_hists_paths, 
  /**/                                           const vector< int    >& t_hists_IDs  ,
  /**/                                           const vector< string >& t_hists_names );
  /**/
  /**/
  /**/// Indices of Refraction
  /**/
  /**/
  /**/
  /**/// Photon transmission
  /**/
  /**/
  /**/
  /**/// Photon emission
  /**/
  /**/
  /**/ 
  /**/// Energy loss
  /**/
  /**/
  /**/
  /**////////////////////////////
  


//   /**/////////////////////////////////////////////////
//   /**/// Expected Individual Detector Measurements ///
//   /**/////////////////////////////////////////////////
//   /**/
//   /**/ inline double get_expected_cherge( Particle* t_particle, RecoDigit* t_digit );
//   /**/ inline double get_expected_time  ( Particle* t_particle, RecoDigit* t_digit );
//   /**/
//   /**/////////////////////////////////////////////////
//   
//   
// 
//   /**///////////////////////
//   /**/// Get Likelyhoods ///
//   /**///////////////////////
//   /**/
//   /**/ inline double get_likelyhood_total       ( Particle* t_particle ); // likelyhood of total event
//   /**/ inline double get_likelyhood_time_total  ( Particle* t_particle ); // total time likelyhood
//   /**/ inline double get_likelyhood_time_PMT    ( Particle* t_particle ); // time likelyhood using all PMTs
//   /**/ inline double get_likelyhood_time_LAPPD  ( Particle* t_particle ); // time likelyhood using all LAPPDs
//   /**/ inline double get_likelyhood_time_MRD    ( Particle* t_particle ); // time likelyhood using MRD
//   /**/ inline double get_likelyhood_charge_total( Particle* t_particle ); // total cherge likelyhood
//   /**/ inline double get_likelyhood_charge_PMT  ( Particle* t_particle ); // charge likelyhood using all PMTs
//   /**/ inline double get_likelyhood_charge_LAPPD( Particle* t_particle ); // charge likelyhood using all LAPPDs
//   /**/ inline double get_likelyhood_charge_MRD  ( Particle* t_particle ); // charge likelyhood using MRD
//   /**/
//   /**///////////////////////



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
    /**/ map< int, TH2D* >* m_hists_emission_mu_water{ nullptr };
    /**/ map< int, TH2D* >* m_hists_emission_e_water { nullptr };
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
    /**/ double             m_refIndex_tankWater     { 1.333   };
    /**/ double             m_refIndex_PMTGlass      { 1.500   };
    /**/ double             m_refIndex_MRDSci        { 1.500   };
    /**/
    /**////////////////////////////
};

#endif