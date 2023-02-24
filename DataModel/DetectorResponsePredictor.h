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
  
  

    /**///////////////////////
    /**/// Other Variables ///
    /**///////////////////////
    /**/
    /**/ void                         set_vtxGeo         ( VertexGeometry* t_vtxGeo ) { m_vtxGeo = t_vtxGeo; }
    /**/ VertexGeometry             * get_vtxGeo         () { return m_vtxGeo; }
    /**/ void                         set_histReader_TH2D( THistReader< double, TH2D >* t_histReader_TH2D ) { m_histReader_TH2D = t_histReader_TH2D; }
    /**/ THistReader< double, TH2D >* get_histReader_TH2D() { return m_histReader_TH2D; }
    /**/ void                         set_histReader_TH1D( THistReader< bool  , TH1D >* t_histReader_TH1D ) { m_histReader_TH1D = t_histReader_TH1D; }
    /**/ THistReader< bool  , TH1D >* get_histReader_TH1D() { return m_histReader_TH1D; }
    /**/
    /**///////////////////////



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
    /**/ virtual double get_expected_height( Particle* t_particle, RecoDigit* t_digit ) = 0;
    /**/ virtual double get_expected_time  ( Particle* t_particle, RecoDigit* t_digit ) = 0;
    /**/
    /**////////////////////////////////////



protected:
    /**////////////////
    /**/// Settings ///
    /**////////////////
    /**/
    /**/ enum m_verbosity_enum{ m_verbosity_error   = 0,
    /**/                        m_verbosity_warning = 1,
    /**/                        m_verbosity_message = 2,
    /**/                        m_verbosity_debug   = 3 };
    /**/ int m_verbosity{ m_verbosity_warning };
    /**/
    /**////////////////



    /**///////////////////////
    /**/// Other Variables ///
    /**///////////////////////
    /**/
    /**/         VertexGeometry             * m_vtxGeo         { VertexGeometry::Instance() };
    /**/         THistReader< double, TH2D >* m_histReader_TH2D{ nullptr };
    /**/         THistReader< bool  , TH1D >* m_histReader_TH1D{ nullptr };
    /**/ mutable string                       m_temp_string;
    /**/
    /**///////////////////////



    /**////////////////////////////
    /**/// Physical Information ///
    /**////////////////////////////
    /**/
    /**/ vector< double        >  m_hists_emission_initialEnergies           ;
    /**/ map   < double, TH2D* >* m_hists_emission_tankWater      { nullptr };
    /**/ map   < double, TH2D* >* m_hists_emission_MRDsci         { nullptr };
    /**/ TH1D                   * m_hist_transmission_tankWater   { nullptr };
    /**/ TH1D                   * m_hist_transmission_MRDsci      { nullptr };
    /**/ TH1D                   * m_hist_dEdX_tankWater           { nullptr };
    /**/ TH1D                   * m_hist_dEdX_tankSteel           { nullptr };
    /**/ TH1D                   * m_hist_dEdX_MRDsci              { nullptr };
    /**/ TH1D                   * m_hist_dEdX_MRDiron             { nullptr };
    /**/
    /**////////////////////////////



    /**////////////////////////////
    /**/// Load/Eval Histograms ///
    /**////////////////////////////
    /**/
    /**/ bool load_hists_emission  (       map   < double, TH2D* >*  t_hists_emission,
    /**/                             const vector< string        > & t_hists_paths   , 
    /**/                             const vector< double        > & t_hists_IDs     ,
    /**/                             const vector< string        > & t_hists_names   );
    /**/ bool load_hist            (       TH1D                   *  t_hist          ,
    /**/                             const string                  & t_hist_path     ,
    /**/                             const string                  & t_hist_name     );
    /**/ double eval_hists_emission(       map   < double, TH2D* >*  t_hists_emission,
    /**/                             const double                  & t_initialEnergy ,
    /**/                             const double                  & t_trackLength   , 
    /**/                             const double                  & t_photonAngle   ) const;
    /**/ double eval_hist          (       TH1D                   *  t_hist          ,
    /**/                             const double                  & t_x             ) const;
    /**/
    /**////////////////////////////



    /**//////////////
    /**/// Output ///
    /**//////////////
    /**/
    /**/ inline void Log_debug( const string&  t_message, const int& t_verbosity ) const;
    /**/ inline void Log_debug( const string&& t_message, const int& t_verbosity ) const;
    /**/
    /**//////////////
};

#endif