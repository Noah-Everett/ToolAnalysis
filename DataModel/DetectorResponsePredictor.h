//////////////////////////////////
// DetectorResponsePrediction.h //
//////////////////////////////////
//
// Noah Everett (noah.everett@mines.sdsmt.edu)
// Created Febuary 13, 2023
//
// Abstract class for generalized detector 
// response prediction.  All information 
// (PDFs, simulations, etc.) needs to be 
// set by the user.
//
// Notation: t_ = function variable
//           m_ = member variable
//           naming: mainVariable_subCategory (use '_' like LaTeX)
//
//////////////////////////////////

#ifndef DETECTORRESPONSEPREDICTION_H
#define DETECTORRESPONSEPREDICTION_H

#include <vector>
#include <string>

#include "TH1.h"
#include "TH2.h"

#include "VertexGeometry.h"
#include "THistReader.h"
#include "Particle.h"
#include "Logging.h"

using std::vector;
using std::string;

class DetectorResponsePredictor
{
public:
    /**////////////////////////
    /**/// (Con/De)structor ///
    /**////////////////////////
    /**/
    /**/ DetectorResponsePredictor() {}
    /**/~DetectorResponsePredictor() { reset_members(); }
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
    /**/ int  get_verbosity() const { return m_verbosity; }
    /**/
    /**/////////////////

  
  
    /**///////////////////
    /**/// Data Models ///
    /**///////////////////
    /**/
    /**/// Vertex Geometry
    /**/ static void            set_vtxGeo( VertexGeometry* t_vtxGeo ) { m_vtxGeo = t_vtxGeo; }
    /**/ static VertexGeometry* get_vtxGeo() { return m_vtxGeo; }
    /**/
    /**///////////////////



    /**////////////////////////////
    /**/// Physical Information ///
    /**////////////////////////////
    /**/
    /**/// Load Histograms
    /**/ inline bool load_hists_emission_tankWater( const vector< string        > & t_hists_paths   , 
    /**/                                            const vector< double        > & t_hists_IDs     ,
    /**/                                            const vector< string        > & t_hists_names   );
    /**/ inline bool load_hists_emission_MRDsci   ( const vector< string        > & t_hists_paths   , 
    /**/                                            const vector< double        > & t_hists_IDs     ,
    /**/                                            const vector< string        > & t_hists_names   );
    /**/
    /**/ inline bool load_hist_transmission_tankWater( const string & t_hist_path,
    /**/                                               const string & t_hist_name );
    /**/ inline bool load_hist_transmission_MRDsci   ( const string & t_hist_path,
    /**/                                               const string & t_hist_name );
    /**/ inline bool load_hist_dEdX_tankWater        ( const string & t_hist_path,
    /**/                                               const string & t_hist_name );
    /**/ inline bool load_hist_dEdX_tankSteel        ( const string & t_hist_path,
    /**/                                               const string & t_hist_name );
    /**/ inline bool load_hist_dEdX_MRDsci           ( const string & t_hist_path,
    /**/                                               const string & t_hist_name );
    /**/ inline bool load_hist_dEdX_MRDiron          ( const string & t_hist_path,
    /**/                                               const string & t_hist_name );
    /**/
    /**/// Evaluate Histograms
    /**/ inline double eval_hists_emission_tankWater( const double               & t_initialEnergy ,
    /**/                                              const double               & t_trackLength   , 
    /**/                                              const double               & t_photonAngle   ) const;
    /**/ inline double eval_hists_emission_MRDsci   ( const double               & t_initialEnergy ,
    /**/                                              const double               & t_trackLength   , 
    /**/                                              const double               & t_photonAngle   ) const;
    /**/
    /**/ inline double eval_hist_transmission_tankWater( const double & t_photonEnergy  ) const;
    /**/ inline double eval_hist_transmission_MRDsci   ( const double & t_photonEnergy  ) const;
    /**/ inline double eval_hist_dEdX_tankWater        ( const double & t_primaryEnergy ) const;
    /**/ inline double eval_hist_dEdX_tankSteel        ( const double & t_primaryEnergy ) const;
    /**/ inline double eval_hist_dEdX_MRDsci           ( const double & t_primaryEnergy ) const;
    /**/ inline double eval_hist_dEdX_MRDiron          ( const double & t_primaryEnergy ) const;
    /**/
    /**////////////////////////////
  


    /**////////////////////////////////////
    /**/// Detector Response Prediction ///
    /**////////////////////////////////////
    /**/
    /**/ double get_expected_height( Particle* t_particle, RecoDigit* t_digit ) = 0;
    /**/ double get_expected_time  ( Particle* t_particle, RecoDigit* t_digit ) = 0;
    /**/
    /**////////////////////////////////////



protected:
    /**////////////////
    /**/// Settings ///
    /**////////////////
    /**/
    /**/ const enum m_verbosity_enum{ m_verbosity_error   = 0,
    /**/                              m_verbosity_warning = 1,
    /**/                              m_verbosity_message = 2,
    /**/                              m_verbosity_debug   = 3 };
    /**/ int m_verbosity{ m_verbosity_warning };
    /**/
    /**////////////////



    /**///////////////////////
    /**/// Other Variables ///
    /**///////////////////////
    /**/
    /**/ static VertexGeometry             * m_vtxGeo         { nullptr };
    /**/ static THistReader< double, TH2D >* m_histReader_TH2D{ nullptr };
    /**/ static THistReader< double, TH1D >* m_histReader_TH1D{ nullptr };
    /**/ static string                       m_temp_string;
    /**/
    /**///////////////////////



    /**////////////////////////////
    /**/// Physical Information ///
    /**////////////////////////////
    /**/
    /**/        vector< double        >  m_hists_emission_initialEnergies           ;
    /**/        map   < double, TH2D* >* m_hists_emission_tankWater      { nullptr };
    /**/        map   < double, TH2D* >* m_hists_emission_MRDsci         { nullptr };
    /**/ static TH1D                * m_hist_transmission_tankWater      { nullptr };
    /**/ static TH1D                * m_hist_transmission_MRDsci         { nullptr };
    /**/        TH1D                * m_hist_dEdX_tankWater              { nullptr };
    /**/        TH1D                * m_hist_dEdX_tankSteel              { nullptr };
    /**/        TH1D                * m_hist_dEdX_MRDsci                 { nullptr };
    /**/        TH1D                * m_hist_dEdX_MRDiron                { nullptr };
    /**/
    /**////////////////////////////



    /**////////////////////////////
    /**/// Load/Eval Histograms ///
    /**////////////////////////////
    /**/
    /**/ bool load_hists_emission  ( const map   < double, TH2D* >*& t_hists_emission,
    /**/                             const vector< string        > & t_hists_paths   , 
    /**/                             const vector< double        > & t_hists_IDs     ,
    /**/                             const vector< string        > & t_hists_names   );
    /**/ bool load_hist            ( const TH1D                   *& t_hist          ,
    /**/                             const string                  & t_hist_path     ,
    /**/                             const string                  & t_hist_name     );
    /**/ double eval_hists_emission( const map   < double, TH2D* >*& t_hists_emission,
    /**/                             const double                  & t_initialEnergy ,
    /**/                             const double                  & t_trackLength   , 
    /**/                             const double                  & t_photonAngle   ) const;
    /**/ double eval_hist          ( const TH1D                   *& t_hist,
    /**/                             const double                  & t_x             ) const;
    /**/
    /**////////////////////////////



    /**//////////////
    /**/// Output ///
    /**//////////////
    /**/
    /**/ inline void Log_debug( string& t_message, int& t_verbosity );
    /**/
    /**//////////////
};

#endif