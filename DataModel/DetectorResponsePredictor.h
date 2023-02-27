//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                DetectorResponsePredictor.h                              //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                                                                         //*//
//*// Author:                                                                                 //*//
//*//   Noah Everett (noah.everett@mines.sdsmt.edu)                                           //*//
//*//   Created Febuary 13, 2023                                                              //*//
//*//                                                                                         //*//
//*// About:                                                                                  //*//
//*//   This file contains classes for generalized detector response predicction. All         //*//
//*//   information (PDFs, histograms, etc.) needs to be set by the user.                     //*//
//*//                                                                                         //*//
//*// Classes:                                                                                //*//
//*//   DetectorResponsePredictor         --> Parent abstract class for the following classes //*//
//*//   DetectorResponsePredictor_TankPMT --> Class for predicting the responses of tank PMTs //*//
//*//   DetectorResponsePredictor_MRDPMT  --> Class for predicting the responses of MRD PMTs  //*//
//*//   DetectorResponsePredictor_LAPPD   --> Class for predicting the responses of LAPPDs    //*//
//*//                                                                                         //*//
//*// Notation:                                                                               //*//
//*//   t_ = function argument                                                                //*//
//*//   m_ = member variable                                                                  //*//
//*//   mainVariable_subCategory (use '_' like LaTeX)                                         //*//
//*//                                                                                         //*//
//*// Abreviations:                                                                           //*//
//*//   ptcl = particle                                                                       //*//
//*//   vtx  = vertex                                                                         //*//
//*//                                                                                         //*//
//*// Units:                                                                                  //*//
//*//   Energy   [MeV]                                                                        //*//
//*//   Distance [m]                                                                          //*//
//*//   Angle    [rad]                                                                        //*//
//*//   Time     [s]                                                                          //*//
//*//                                                                                         //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//

#ifndef DETECTORRESPONSEPREDICTOR_H
#define DETECTORRESPONSEPREDICTOR_H

#include <vector>
#include <string>

#include "TH1.h"
#include "TH2.h"

#include "VertexGeometry.h"
#include "THistReader.h"
#include "TVector3.h"
#include "TVector2.h"
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
  
  
  
    /**/////////////////////////
    /**/// Get/Set Variables ///
    /**/////////////////////////
    /**/
    /**/ void set_verbosity( int t_verbosity ) { m_verbosity = t_verbosity; }
    /**/ int  get_verbosity() const            { return m_verbosity;        }
    /**/
    /**/ void set_nPoints( int t_nPoints ) { m_nPoints = t_nPoints; }
    /**/ int  get_nPoints() const          { return m_nPoints;      }
    /**/
    /**/ void set_nIterations( int t_nIterations ) { m_nIterations = t_nIterations; }
    /**/ int  get_nIterations() const              { return m_nIterations;          }
    /**/
    /**/ void            set_vtxGeo( VertexGeometry* t_vtxGeo ) { m_vtxGeo = t_vtxGeo; }
    /**/ VertexGeometry* get_vtxGeo(                          ) { return m_vtxGeo;     }
    /**/
    /**/ void                      set_histReader_TH2D( THistReader< int, TH2D >* t_histReader_TH2D ) { m_histReader_TH2D = t_histReader_TH2D; }
    /**/ THistReader< int, TH2D >* get_histReader_TH2D(                                             ) { return m_histReader_TH2D;              }
    /**/
    /**/ void                         set_histReader_TH1D( THistReader< bool  , TH1D >* t_histReader_TH1D ) { m_histReader_TH1D = t_histReader_TH1D; }
    /**/ THistReader< bool  , TH1D >* get_histReader_TH1D(                                                ) { return m_histReader_TH1D;              }
    /**/
    /**/ void reset_members();
    /**/
    /**/////////////////////////



    /**////////////////////////////
    /**/// Physical Information ///
    /**////////////////////////////
    /**/
    /**/// Load Histograms
    /**/ inline bool load_hists_emission_tankWater( const vector< string >& t_hists_paths, 
    /**/                                            const vector< int    >& t_hists_IDs  ,
    /**/                                            const vector< string >& t_hists_names );
    /**/ inline bool load_hists_emission_MRDsci   ( const vector< string >& t_hists_paths, 
    /**/                                            const vector< int    >& t_hists_IDs  ,
    /**/                                            const vector< string >& t_hists_names );
    /**/
    /**/ inline bool load_hist_transmission_tankWater( const string& t_hist_path,
    /**/                                               const string& t_hist_name );
    /**/ inline bool load_hist_transmission_MRDsci   ( const string& t_hist_path,
    /**/                                               const string& t_hist_name );
    /**/ inline bool load_hist_dEdX_tankWater        ( const string& t_hist_path,
    /**/                                               const string& t_hist_name );
    /**/ inline bool load_hist_dEdX_tankSteel        ( const string& t_hist_path,
    /**/                                               const string& t_hist_name );
    /**/ inline bool load_hist_dEdX_MRDsci           ( const string& t_hist_path,
    /**/                                               const string& t_hist_name );
    /**/ inline bool load_hist_dEdX_MRDiron          ( const string& t_hist_path,
    /**/                                               const string& t_hist_name );
    /**/
    /**/// Evaluate Histograms
    /**/ inline double eval_hists_emission_tankWater( const double t_initialEnergy,
    /**/                                              const double t_trackLength  , 
    /**/                                              const double t_photonAngle   ) const;
    /**/ inline double eval_hists_emission_MRDsci   ( const double t_initialEnergy,
    /**/                                              const double t_trackLength  , 
    /**/                                              const double t_photonAngle   ) const;
    /**/
    /**/ inline double eval_hist_transmission_tankWater( const double t_photonEnergy  ) const;
    /**/ inline double eval_hist_transmission_MRDsci   ( const double t_photonEnergy  ) const;
    /**/ inline double eval_hist_dEdX_tankWater        ( const double t_primaryEnergy ) const;
    /**/ inline double eval_hist_dEdX_tankSteel        ( const double t_primaryEnergy ) const;
    /**/ inline double eval_hist_dEdX_MRDsci           ( const double t_primaryEnergy ) const;
    /**/ inline double eval_hist_dEdX_MRDiron          ( const double t_primaryEnergy ) const;
    /**/
    /**////////////////////////////
  


    /**////////////////////////////////////
    /**/// Detector Response Prediction ///
    /**////////////////////////////////////
    /**/
    /**/ virtual double get_expected_height( Particle* t_particle, int t_nDigit ) = 0;
    /**/ virtual double get_expected_time  ( Particle* t_particle, int t_nDigit ) = 0;
    /**/
    /**////////////////////////////////////



protected:
    /**//////////////
    /**/// Output ///
    /**//////////////
    /**/
    /**/ enum m_verbosity_enum{ m_verbosity_error   = 0,
    /**/                        m_verbosity_warning = 1,
    /**/                        m_verbosity_message = 2,
    /**/                        m_verbosity_debug   = 3 };
    /**/
    /**/ inline void Log_debug( const string&  t_message, const int  t_verbosity ) const;
    /**/ inline void Log_debug( const string&& t_message, const int  t_verbosity ) const;
    /**/
    /**//////////////


    
    /**////////////////
    /**/// Settings ///
    /**////////////////
    /**/
    /**/ int    m_verbosity      { m_verbosity_warning };
    /**/ int    m_nPoints        { 4                   };
    /**/ int    m_nIterations    { 10                  };
    /**/ double m_PMTdiameter    { 8 * 0.0254          };
    /**/ double m_LAPPDsideLength{ 20 * 0.01           };
    /**/
    /**////////////////



    /**//////////////////
    /**/// DataModels ///
    /**//////////////////
    /**/
    /**/ VertexGeometry           * m_vtxGeo         { VertexGeometry::Instance() };
    /**/ THistReader< int , TH2D >* m_histReader_TH2D{ nullptr };
    /**/ THistReader< bool, TH1D >* m_histReader_TH1D{ nullptr };
    /**/
    /**//////////////////



    /**/////////////////////////////////
    /**/// Miscilanious Helpful Bits ///
    /**/////////////////////////////////
    /**/
    /**/// Temporary Variables
    /**/ mutable string   m_temp_string;
    /**/ mutable TVector3 m_temp_TVector3;
    /**/ mutable TVector2 m_temp_TVector2;
    /**/
    /**/// Copy Functions (as to not copy by reference)
    /**/ inline TVector3 copy( const TVector3& t_TVector3 ) const;
    /**/ inline TVector2 copy( const TVector2& t_TVector2 ) const;
    /**/
    /**/////////////////////////////////



    /**////////////////////////////
    /**/// Physical Information ///
    /**////////////////////////////
    /**/
    /**/ vector< int        >  m_hists_emission_initialEnergies           ;
    /**/ map   < int, TH2D* >* m_hists_emission_tankWater      { nullptr };
    /**/ map   < int, TH2D* >* m_hists_emission_MRDsci         { nullptr };
    /**/ TH1D                * m_hist_transmission_tankWater   { nullptr };
    /**/ TH1D                * m_hist_transmission_MRDsci      { nullptr };
    /**/ TH1D                * m_hist_dEdX_tankWater           { nullptr };
    /**/ TH1D                * m_hist_dEdX_tankSteel           { nullptr };
    /**/ TH1D                * m_hist_dEdX_MRDsci              { nullptr };
    /**/ TH1D                * m_hist_dEdX_MRDiron             { nullptr };
    /**/
    /**////////////////////////////



    /**////////////////////////////
    /**/// Load/Eval Histograms ///
    /**////////////////////////////
    /**/
    /**/// Load Histograms
    /**/ bool load_hists_emission( const map   < int, TH2D* >* t_hists_emission,
    /**/                           const vector< string     >& t_hists_paths   , 
    /**/                           const vector< int        >& t_hists_IDs     ,
    /**/                           const vector< string     >& t_hists_names   );
    /**/ bool load_hist          ( const TH1D                * t_hist          ,
    /**/                           const string              & t_hist_path     ,
    /**/                           const string              & t_hist_name     );
    /**/
    /**/// Evaluate Histograms
    /**/ double eval_hists_emission( const map< int, TH2D* >* t_hists_emission,
    /**/                             const double             t_initialEnergy ,
    /**/                             const double             t_trackLength   , 
    /**/                             const double             t_photonAngle   ) const;
    /**/ double eval_hist          ( const TH1D             * t_hist          ,
    /**/                             const double             t_x             ) const;
    /**/
    /**////////////////////////////
    
    
    
    /**/////////////////////////////
    /**/// Calculation Functions ///
    /**/////////////////////////////
    /**/
    /**/ inline double                  get_angle             ( const TVector3& t_vector_1      , const TVector3& t_vector_2         ) const;
    /**/ inline double                  get_distance          ( const TVector3& t_point_1       , const TVector3& t_point_2          ) const;
    /**/        pair< double, double >* get_emissionHistPoints( const TVector3& t_point         , const TVector3& t_ptcl_init        , 
    /**/                                                        const TVector3& t_ptcl_direction, const double    t_ptcl_distance_max,
    /**/                                                        const int       t_nIterations                                        ) const;
    /**/
    /**/////////////////////////////
};



class DetectorResponsePredictor_TankPMT : public DetectorResponsePredictor
{
public:
    double get_expected_height( const Particle* t_particle, const int t_nDigit );
    double get_expected_time  ( const Particle* t_particle, const int t_nDigit );
};



class DetectorResponsePredictor_MRDPMT : public DetectorResponsePredictor
{
public:
    // double get_expected_height( Particle* t_particle, int t_nDigit );
    // double get_expected_time  ( Particle* t_particle, int t_nDigit );
};



class DetectorResponsePredictor_LAPPD : public DetectorResponsePredictor
{
public:
    // double get_expected_height( Particle* t_particle, int t_nDigit );
    // double get_expected_time  ( Particle* t_particle, int t_nDigit );
};

#endif