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
//*// Units:                                                                                  //*//
//*//   Energy        [MeV]                                                                   //*//
//*//   Distance      [m]                                                                     //*//
//*//   Angle         [rad]                                                                   //*//
//*//   Time          [s]                                                                     //*//
//*//   Transmittance [1/m] (%/m)                                                             //*//
//*//                                                                                         //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                                                                         //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//

#ifndef DETECTORRESPONSEPREDICTOR_H
#define DETECTORRESPONSEPREDICTOR_H

// STD
#include <vector>
#include <queue>
#include <string>
#include <cmath>

// ROOT
#include "TH1.h"
#include "TH2.h"
#include "TVector3.h"
#include "TVector2.h"

// DataModels
#include "VertexGeometry.h"
#include "THistReader.h"
#include "Particle.h"
#include "Detector.h"

using std::vector;
using std::string;
    
// Index Structures
struct index_3{ unsigned int x, y, z; };
struct index_2{ unsigned int x, y   ; };

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
    /**/ void         set_verbosity( unsigned int t_verbosity ) { m_verbosity = t_verbosity; }
    /**/ unsigned int get_verbosity() const                     { return m_verbosity;        }
    /**/
    /**/ void reset_members();
    /**/
    /**/////////////////////////



    /**////////////////////////////
    /**/// Physical Information ///
    /**////////////////////////////
    /**/
    /**/// Load Histograms
    /**/ bool load_hists_emission_tankWater         ( const vector< string >& t_hists_paths, 
    /**/                                                     const vector< int    >& t_hists_IDs  ,
    /**/                                                     const vector< string >& t_hists_names );
    /**/ bool load_hists_emission_MRDsci            ( const vector< string >& t_hists_paths, 
    /**/                                                     const vector< int    >& t_hists_IDs  ,
    /**/                                                     const vector< string >& t_hists_names );
    /**/ bool load_hists_emission_tankWater_energies( const vector< string >& t_hists_paths,
    /**/                                                     const vector< int    >& t_hists_IDs  ,
    /**/                                                     const vector< string >& t_hists_names );
    /**/ bool load_hists_emission_MRDsci_energies   ( const vector< string >& t_hists_paths,
    /**/                                                     const vector< int    >& t_hists_IDs  ,
    /**/                                                     const vector< string >& t_hists_names );
    /**/
    /**/ bool load_hist_transmission_tankWater( const string& t_hist_path,
    /**/                                               const string& t_hist_name );
    /**/ bool load_hist_transmission_MRDsci   ( const string& t_hist_path,
    /**/                                               const string& t_hist_name );
    /**/ bool load_hist_dEdX_tankWater        ( const string& t_hist_path,
    /**/                                               const string& t_hist_name );
    /**/ bool load_hist_dEdX_tankSteel        ( const string& t_hist_path,
    /**/                                               const string& t_hist_name );
    /**/ bool load_hist_dEdX_MRDsci           ( const string& t_hist_path,
    /**/                                               const string& t_hist_name );
    /**/ bool load_hist_dEdX_MRDiron          ( const string& t_hist_path,
    /**/                                               const string& t_hist_name );
    /**/
    /**/// Evaluate Histograms
    /**/ double eval_hists_emission_tankWater( const double t_initialEnergy,
    /**/                                              const double t_trackLength  , 
    /**/                                              const double t_photonAngle   ) const;
    /**/ double eval_hists_emission_MRDsci   ( const double t_initialEnergy,
    /**/                                              const double t_trackLength  , 
    /**/                                              const double t_photonAngle   ) const;
    /**/
    /**/ double eval_hist_transmission_tankWater( const double t_photonEnergy  ) const;
    /**/ double eval_hist_transmission_MRDsci   ( const double t_photonEnergy  ) const;
    /**/ double eval_hist_dEdX_tankWater        ( const double t_primaryEnergy ) const;
    /**/ double eval_hist_dEdX_tankSteel        ( const double t_primaryEnergy ) const;
    /**/ double eval_hist_dEdX_MRDsci           ( const double t_primaryEnergy ) const;
    /**/ double eval_hist_dEdX_MRDiron          ( const double t_primaryEnergy ) const;
    /**/
    /**/// Misc.
    /**/ void   load_map_particleMasses( map< int, double >* t_map_particleMasses ) { m_map_particleMasses = t_map_particleMasses; }
    /**/ double eval_map_particleMasses( int                 t_PDG                ) { return m_map_particleMasses->at( t_PDG );    }
    /**/
    /**/ void   set_c( double t_c ) { m_c = t_c;  }
    /**/ double get_c()             { return m_c; }
    /**/
    /**////////////////////////////
  


    /**////////////////////////////////////
    /**/// Detector Response Prediction ///
    /**////////////////////////////////////
    /**/
    /**/ double get_expected_height( Particle* t_particle, Detector* t_detector, double t_time_start, double t_time_stop ) const;
    /**/ double get_expected_time  ( Particle* t_particle, Detector* t_detector ) const;
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
    /**/ void Log_debug( const string&  t_message, const unsigned int t_verbosity ) const;
    /**/ void Log_debug( const string&& t_message, const unsigned int t_verbosity ) const;
    /**/
    /**//////////////


    
    /**////////////////
    /**/// Settings ///
    /**////////////////
    /**/
    /**/ unsigned int m_verbosity      { m_verbosity_warning };
    /**/ double       m_PMTdiameter    { 8 * 0.0254          };
    /**/ double       m_LAPPDsideLength{ 20 * 0.01           };
    /**/
    /**////////////////



    /**////////////////////////////
    /**/// Physical Information ///
    /**////////////////////////////
    /**/
    /**/// Emission Histograms
    /**/ vector< int        >  m_hists_emission_initialEnergies;
    /**/ map   < int, TH2D* >* m_hists_emission_tankWater;
    /**/ map   < int, TH2D* >* m_hists_emission_MRDsci;
    /**/ map   < int, TH2D* >* m_hists_emission_tankWater_energies;
    /**/ map   < int, TH2D* >* m_hists_emission_MRDsci_energies;
    /**/ double                m_binWidth_s_tankWater;
    /**/ double                m_binWidth_s_MRDsci;
    /**/ double                m_binWidth_theta_tankWater;
    /**/ double                m_binWidth_theta_MRDsci;
    /**/ double                m_binWidth_phi_tankWater;
    /**/ double                m_binWidth_phi_MRDsci;
    /**/
    /**/// Other Histograms
    /**/ TH1D* m_hist_transmission_tankWater;
    /**/ TH1D* m_hist_transmission_MRDsci;
    /**/ TH1D* m_hist_dEdX_tankWater;
    /**/ TH1D* m_hist_dEdX_tankSteel;
    /**/ TH1D* m_hist_dEdX_MRDsci;
    /**/ TH1D* m_hist_dEdX_MRDiron;
    /**/
    /**/// Misc.
    /**/ map< int, double >* m_map_particleMasses{ new map< int, double >{ { 11, 0.5109989461 }, { -11, 0.5109989461 }, 
    /**/                                           { 13, 105.6583755  }, { -13, 105.6583755  } } };
    /**/ double              m_c{ 299792458 };
    /**/
    /**////////////////////////////



    /**////////////////////////////
    /**/// Load/Eval Histograms ///
    /**////////////////////////////
    /**/
    /**/// Load Histograms
    /**/ bool load_hists_emission(       map   < int, TH2D* >* t_hists_emission,
    /**/                           const vector< string     >& t_hists_paths   , 
    /**/                           const vector< int        >& t_hists_IDs     ,
    /**/                           const vector< string     >& t_hists_names   ,
    /**/                                 double              & t_binWidth_s    ,
    /**/                                 double              & t_binWidth_theta,
    /**/                                 double              & t_binWidth_phi   );
    /**/ template< typename type_hist >
    /**/ bool load_hist          ( const type_hist           * t_hist          ,
    /**/                           const string              & t_hist_path     ,
    /**/                           const string              & t_hist_name      );
    /**/
    /**/// Evaluate Histograms
    /**/ pair< int, int > get_closestEmissionHists     ( const map< int, TH2D* >* t_hists_emission  ,
    /**/                                                 const double             t_initialEnergy    ) const;
    /**/ double           eval_hists_emission_values   ( const map< int, TH2D* >* t_hists_emission  ,
    /**/                                                 const double             t_initialEnergy   ,
    /**/                                                 const double             t_trackLength     ,
    /**/                                                 const double             t_photonAngle      ) const;
    /**/ double           eval_hists_emission_indicies ( const map< int, TH2D* >* t_hists_emission  ,
    /**/                                                 const double             t_initialEnergy   ,
    /**/                                                 const unsigned int       t_trackLengthIndex,
    /**/                                                 const unsigned int       t_photonAngleIndex ) const;
    /**/ double           eval_hist_value              ( const TH1D             * t_hist            ,
    /**/                                                 const double             t_x                ) const;
    /**/ double           eval_hist_index              ( const TH1D             * t_hist            ,
    /**/                                                 const unsigned int       t_xIndex           ) const;
    /**/
    /**////////////////////////////
    
    
    
    /**//////////////////////////////
    /**/// Helpful TVector Things ///
    /**//////////////////////////////
    /**/
    /**/ double get_angle   ( const TVector3& t_vector_1, const TVector3& t_vector_2 ) const;
    /**/ double get_distance( const TVector3& t_point_1 , const TVector3& t_point_2  ) const;
    /**/
    /**/ void normalize( TVector3& t_vector ) const;
    /**/
    /**/ const TVector3 m_vector_x{ 1, 0, 0 };
    /**/ const TVector3 m_vector_y{ 0, 1, 0 };
    /**/ const TVector3 m_vector_z{ 0, 0, 1 };
    /**/
    /**//////////////////////////////
    
    
    
    /**////////////////////////////////////
    /**/// Emission Hists Bin Functions ///
    /**////////////////////////////////////
    /**/
    /**/ int get_bin_index( const double t_value, const double t_min  , 
    /**/                    const double t_max  , const int    t_nBins ) const;
    /**/ int get_bin_index( const double t_value, const TAxis* t_axis  ) const;
    /**/
    /**/ index_3            get_bin_index_detector_center  ( const TVector3& t_particle_position_init, const TVector3& t_particle_direction, 
    /**/                                                     const TVector3& t_detector_position     , const TH2D    * t_hist_emission     ,
    /**/                                                     const double    t_particle_trackLength                                    ) const;
    /**/ vector< index_3 >* get_bin_indicies_detector_total( const TH2D    * t_hist_emission    , bool ( *t_get_isDetector )( const index_3 ),
    /**/                                                     const index_3   t_index_start      , const index_3 t_index_max            ) const;
    /**/
    /**/ double get_bin_value( const unsigned int t_index, const double  t_min  , 
    /**/                       const double       t_max  , const int     t_nBins ) const;
    /**/ double get_bin_value( const unsigned int t_index, const TAxis*  t_axis  ) const;
    /**/
    /**////////////////////////////////////


    
    /**/////////////////////////////
    /**/// Is Detector Functions ///
    /**/////////////////////////////
    /**/
    /**/ bool get_isDetector_tankPMT( const TVector3& t_particle_position_init, const TVector3& t_particle_direction,
    /**/                              const TVector3& t_detector_position     , const TVector3& t_detector_direction,
    /**/                              const TH2D    * t_hist_emission         , const index_3   t_binIndex           ) const;
    /**/ bool get_isDetector_LAPPD  ( const TVector3& t_particle_position_init, const TVector3& t_particle_direction,
    /**/                              const TVector3& t_detector_position     , const TVector3& t_detector_direction,
    /**/                              const TH2D    * t_hist_emission         , const index_3   t_binIndex           ) const;
    /**/ bool get_isDetector_MRD    ( const TVector3& t_particle_position_init, const TVector3& t_particle_direction,
    /**/                              const TVector3& t_detector_position     , const TVector3& t_detector_direction,
    /**/                              const TH2D    * t_hist_emission         , const index_3   t_binIndex           ) const;
    /**/
    /**/////////////////////////////



    /**////////////////////////////
    /**/// Acceptance Functions ///
    /**////////////////////////////
    /**/
    /**/ double get_acceptance_tankPMT( const double t_incidenceAngle, const int t_detectorID ) const;
    /**/ double get_acceptance_LAPPD  ( const double t_incidenceAngle, const int t_detectorID ) const;
    /**/ double get_acceptance_MRD    ( const double t_incidenceAngle, const int t_detectorID ) const;
    /**/
    /**////////////////////////////



    /**///////////////////////////////
    /**/// Transmittance Functions ///
    /**///////////////////////////////
    /**/
    /**/ double get_transmittance_tankWater( const double t_distance, const double t_photonEnergy ) const;
    /**/ double get_transmittance_MRDsci   ( const double t_distance, const double t_photonEnergy ) const;
    /**/
    /**///////////////////////////////
    
    
    
    /**//////////////////////////
    /**/// Physical Functions ///
    /**//////////////////////////
    /**/
    /**/ double       get_particle_mass         ( Particle* t_particle                ) const;
    /**/ double       get_particlePosition_value( Particle* t_particle, double t_time ) const;
    /**/ unsigned int get_particlePosition_index( Particle* t_particle, double t_time ) const;
    /**/
    /**//////////////////////////
};

#endif