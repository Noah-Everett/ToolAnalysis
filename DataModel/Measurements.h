//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                      Measurements.h                                     //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                                                                         //*//
//*// Author:                                                                                 //*//
//*//   Noah Everett (noah.everett@mines.sdsmt.edu)                                           //*//
//*//   Created Aug 7, 2024                                                                   //*//
//*//                                                                                         //*//
//*// About:                                                                                  //*//
//*//   This file contains a general class for measurements which include a value and error   //*//
//*//   with units. There are spesific classes for energy, length, time, mass, and angle. I   //*//
//*//   appologize if something like this already exists in ToolAnalysis but my brief search  //*//
//*//   did not find it (:                                                                    //*//
//*//                                                                                         //*//
//*// Notation:                                                                               //*//
//*//   t_ = function argument                                                                //*//
//*//   m_ = member variable                                                                  //*//
//*//   mainVariable_subCategory (use '_' like LaTeX)                                         //*//
//*//                                                                                         //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                                                                         //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//

#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include <string>
#include <iostream>
#include <cmath>
#include <map>

using std::string;
using std::ostream;
using std::pair;
using std::map;
using std::pow;
using std::abs;

template< typename MeasurementType >
class Measurement
{
public:
    Measurement() {}
    Measurement( const Measurement& t_measurement );
    Measurement( double t_value  , const string& t_value_unit_given="DEFAULT", const string& t_value_unit_use="DEFAULT",
                 double t_error=0, const string& t_error_unit_given="DEFAULT", const string& t_error_unit_use="DEFAULT" );
   ~Measurement() {};

    MeasurementType& set_value           ( double t_value, const string& t_value_unit="DEFAULT" ) { return ( *this ).update_value( t_value      , t_value_unit   ); }
    MeasurementType& set_value           ( const pair< double, string >& t_value                ) { return ( *this ).update_value( t_value.first, t_value.second ); }
    MeasurementType& set_value_unit      ( const string& t_value_unit="DEFAULT"                 ) { return ( *this ).update_value(                t_value_unit   ); }
    MeasurementType& set_value_unit_smart( unsigned int t_nMaxDigitsIntegerPart=2               ) { return ( *this ).update_value_smart( t_nMaxDigitsIntegerPart ); }
    MeasurementType& set_error           ( double t_error, const string& t_error_unit="DEFAULT" ) { return ( *this ).update_error( t_error      , t_error_unit   ); }
    MeasurementType& set_error           ( const pair< double, string >& t_error                ) { return ( *this ).update_error( t_error.first, t_error.second ); }
    MeasurementType& set_error_unit      ( const string& t_error_unit="DEFAULT"                 ) { return ( *this ).update_error(                t_error_unit   ); }
    MeasurementType& set_error_unit_smart( unsigned int t_nMaxDigitsIntegerPart=2               ) { return ( *this ).update_error_smart( t_nMaxDigitsIntegerPart ); }
    MeasurementType& set_units           ( const string& t_unit="DEFAULT"                       ) { return ( *this ).update_value(                t_unit         )
                                                                                                                    .update_error(                t_unit         ); }
    MeasurementType& set_units_smart     ( unsigned int t_nMaxDigitsIntegerPart=2               ) { return ( *this ).update_value_smart( t_nMaxDigitsIntegerPart )
                                                                                                                    .update_error_smart( t_nMaxDigitsIntegerPart ); }

    double get_value              ( const string& t_unit=""                ) const { return get_updated_value( t_unit.compare( "" ) ? t_unit : m_value_unit_name ); }
    string get_value_unit         (                                        ) const { return m_value_unit_name;                                                      }
    double get_value_unit_relative( const string& t_unit="DEFAULT"         ) const { return m_value_unit / MeasurementType::m_unitMap.at( t_unit );                 }
    double get_value_unit_smart   ( unsigned int t_nMaxDigitsIntegerPart=2 ) const { return get_updated_value_smart( t_nMaxDigitsIntegerPart );                     }
    double get_error              ( const string& t_unit=""                ) const { return get_updated_error( t_unit.compare( "" ) ? t_unit : m_error_unit_name ); }
    string get_error_unit         (                                        ) const { return m_error_unit_name;                                                      }
    double get_error_unit_relative( const string& t_unit="DEFAULT"         ) const { return m_error_unit / MeasurementType::m_unitMap.at( t_unit );                 }
    double get_error_unit_smart   ( unsigned int t_nMaxDigitsIntegerPart=2 ) const { return get_updated_error_smart( t_nMaxDigitsIntegerPart );                     }

    MeasurementType& operator+( const Measurement& t_measurement ) const { return Measurement( t_measurement ).set_value( m_value_unit ).set_value( m_value + t_measurement.get_value() ); }
    MeasurementType& operator-( const Measurement& t_measurement ) const { return Measurement( t_measurement ).set_value( m_value_unit ).set_value( m_value - t_measurement.get_value() ); }
    MeasurementType& operator*( const Measurement& t_measurement ) const { return Measurement( t_measurement ).set_value( m_value_unit ).set_value( m_value * t_measurement.get_value() ); }
    MeasurementType& operator/( const Measurement& t_measurement ) const { return Measurement( t_measurement ).set_value( m_value_unit ).set_value( m_value / t_measurement.get_value() ); }
    MeasurementType& operator+( const double       t_value       ) const { return Measurement( *this ).set_value( m_value + t_value ); }
    MeasurementType& operator-( const double       t_value       ) const { return Measurement( *this ).set_value( m_value - t_value ); }
    MeasurementType& operator*( const double       t_value       ) const { return Measurement( *this ).set_value( m_value * t_value ); }
    MeasurementType& operator/( const double       t_value       ) const { return Measurement( *this ).set_value( m_value / t_value ); }

    MeasurementType& operator+=( const Measurement& t_measurement ) { m_value += t_measurement.get_value(); return *this; }
    MeasurementType& operator-=( const Measurement& t_measurement ) { m_value -= t_measurement.get_value(); return *this; }
    MeasurementType& operator*=( const Measurement& t_measurement ) { m_value *= t_measurement.get_value(); return *this; }
    MeasurementType& operator/=( const Measurement& t_measurement ) { m_value /= t_measurement.get_value(); return *this; }
    MeasurementType& operator+=( const double       t_value       ) { m_value += t_value; return *this; }
    MeasurementType& operator-=( const double       t_value       ) { m_value -= t_value; return *this; }
    MeasurementType& operator*=( const double       t_value       ) { m_value *= t_value; return *this; }
    MeasurementType& operator/=( const double       t_value       ) { m_value /= t_value; return *this; }

    MeasurementType& operator=( const MeasurementType       & t_measurement ) { return ( *this ).update_value( t_measurement.get_value(), t_measurement.get_value_unit() )
                                                                                                .update_error( t_measurement.get_error(), t_measurement.get_error_unit() ); }
    MeasurementType& operator=( const double                  t_value       ) { return ( *this ).update_value( t_value                       ); }
    MeasurementType& operator=( const pair< double, string >  t_value       ) { return ( *this ).update_value( t_value.first, t_value.second ); }

    bool operator==( const MeasurementType& t_measurement ) const { return m_value == t_measurement.get_value( m_value_unit_name ) && m_error == t_measurement.get_error( m_error_unit_name ); }
    bool operator!=( const MeasurementType& t_measurement ) const { return m_value != t_measurement.get_value( m_value_unit_name ) || m_error != t_measurement.get_error( m_error_unit_name ); }
    bool operator< ( const MeasurementType& t_measurement ) const { return m_value <  t_measurement.get_value( m_value_unit_name ); }
    bool operator<=( const MeasurementType& t_measurement ) const { return m_value <= t_measurement.get_value( m_value_unit_name ); }
    bool operator> ( const MeasurementType& t_measurement ) const { return m_value >  t_measurement.get_value( m_value_unit_name ); }
    bool operator>=( const MeasurementType& t_measurement ) const { return m_value >= t_measurement.get_value( m_value_unit_name ); }
    bool operator==( const double           t_value       ) const { return m_value == t_value; }
    bool operator!=( const double           t_value       ) const { return m_value != t_value; }
    bool operator< ( const double           t_value       ) const { return m_value <  t_value; }
    bool operator<=( const double           t_value       ) const { return m_value <= t_value; }
    bool operator> ( const double           t_value       ) const { return m_value >  t_value; }
    bool operator>=( const double           t_value       ) const { return m_value >= t_value; }
    
    bool operator()( const MeasurementType& t_measurement_0, const MeasurementType& t_measurement_1, double t_epsilon=1e-6 ) const; // For use in std::sort or std::map

    operator double() const { return m_value; }

    friend ostream& operator<<( ostream& t_os, const MeasurementType& t_measurement ) { t_os << t_measurement.get_value(); return t_os; }
    ostream& print( ostream& t_os ) const { t_os << m_value << " " << m_value_unit_name << " +/- " << m_error << " " << m_error_unit_name; return t_os; }

protected:
    double m_value          { 0         };
    double m_value_unit     { 1         };
    string m_value_unit_name{ "DEFAULT" };
    double m_error          { 0         };
    double m_error_unit     { 1         };
    string m_error_unit_name{ "DEFAULT" };

    inline static const double pi = 3.14159265358979;

private:
    double get_updated_value( double t_value, const string& t_unit_given, const string& t_unit_use ) const;
    double get_updated_value( double t_value, const string& t_value_unit ) const { return get_updated_value( t_value, m_value_unit_name, t_value_unit ); }
    double get_updated_error( double t_error, const string& t_error_unit ) const { return get_updated_value( t_error, m_error_unit_name, t_error_unit ); }
    double get_updated_value( const string& t_value_unit ) const { return get_updated_value( m_value, t_value_unit ); }
    double get_updated_error( const string& t_error_unit ) const { return get_updated_error( m_error, t_error_unit ); }

    pair< double, string > get_updated_value_smart( double t_value, const string& t_unit_given, unsigned int t_nMaxDigitsIntegerPart=2 ) const;
    double get_updated_value_smart     ( unsigned int t_nMaxDigitsIntegerPart=2 ) const { return get_updated_value_smart( m_value, m_value_unit_name, t_nMaxDigitsIntegerPart ).first;  }
    double get_updated_error_smart     ( unsigned int t_nMaxDigitsIntegerPart=2 ) const { return get_updated_value_smart( m_error, m_error_unit_name, t_nMaxDigitsIntegerPart ).first;  }
    string get_updated_value_unit_smart( unsigned int t_nMaxDigitsIntegerPart=2 ) const { return get_updated_value_smart( m_value, m_value_unit_name, t_nMaxDigitsIntegerPart ).second; }
    string get_updated_error_unit_smart( unsigned int t_nMaxDigitsIntegerPart=2 ) const { return get_updated_value_smart( m_error, m_error_unit_name, t_nMaxDigitsIntegerPart ).second; }

    MeasurementType& update_value( double t_value, const string& t_value_unit );
    MeasurementType& update_error( double t_error, const string& t_error_unit );
    MeasurementType& update_value( const string& t_value_unit ) { return ( *this ).update_value( m_value, t_value_unit ); }
    MeasurementType& update_error( const string& t_error_unit ) { return ( *this ).update_error( m_error, t_error_unit ); }
    MeasurementType& update_value( double t_value ) { m_value = t_value; return *this; }
    MeasurementType& update_error( double t_error ) { m_error = t_error; return *this; }

    MeasurementType& update_value_smart( unsigned int t_nMaxDigitsIntegerPart=2 ) { return ( *this ).update_value( get_updated_value_smart( t_nMaxDigitsIntegerPart ) ); }
    MeasurementType& update_error_smart( unsigned int t_nMaxDigitsIntegerPart=2 ) { return ( *this ).update_error( get_updated_error_smart( t_nMaxDigitsIntegerPart ) ); }
};

class Energy : public Measurement< Energy >
{
public:
    Energy() : Measurement() {}
    Energy( const Energy& t_energy ) : Measurement( t_energy ) {}
    Energy( double t_value, const string& t_value_unit_given="DEFAULT", const string& t_value_unit_use="DEFAULT",
            double t_error=0, const string& t_error_unit_given="DEFAULT", const string& t_error_unit_use="DEFAULT" ) :
        Measurement( t_value, t_value_unit_given, t_value_unit_use, t_error, t_error_unit_given, t_error_unit_use ) {}
   ~Energy() {}

protected:
    inline static map< string, double > m_unitMap = {
        { "DEFAULT", 1e+0      },

        { "peV"    , 1e-12     },
        { "meV"    , 1e-3      },
        { "ceV"    , 1e-2      },
        { "deV"    , 1e-1      },
        { "eV"     , 1e+0      },
        { "keV"    , 1e+3      },
        { "MeV"    , 1e+6      },
        { "GeV"    , 1e+9      },
        { "TeV"    , 1e+12     },
        { "PeV"    , 1e+15     },

        { "J"      , 6.242e+12 }
    };
};

class Length : public Measurement< Length >
{
public:
    Length() : Measurement() {}
    Length( const Length& t_length ) : Measurement( t_length ) {}
    Length( double t_value, const string& t_value_unit_given="DEFAULT", const string& t_value_unit_use="DEFAULT",
            double t_error=0, const string& t_error_unit_given="DEFAULT", const string& t_error_unit_use="DEFAULT" ) :
        Measurement( t_value, t_value_unit_given, t_value_unit_use, t_error, t_error_unit_given, t_error_unit_use ) {}
   ~Length() {}

protected:
    inline static map< string, double > m_unitMap = {
        { "DEFAULT", 1e+0    },

        { "pm"     , 1e-12   },
        { "nm"     , 1e-9    },
        { "um"     , 1e-6    },
        { "mm"     , 1e-3    },
        { "cm"     , 1e-2    },
        { "dm"     , 1e-1    },
        { "m"      , 1e+0    },
        { "km"     , 1e+3    },
        { "Mm"     , 1e+6    },
        { "Gm"     , 1e+9    },
        { "Tm"     , 1e+12   },
        { "Pm"     , 1e+15   },

        { "in"     , 0.0254  },
        { "ft"     , 0.3048  },
        { "yd"     , 0.9144  },
        { "mi"     , 1609.34 }
    };
};

class Time : public Measurement< Time >
{
public:
    Time() : Measurement() {}
    Time( const Time& t_time ) : Measurement( t_time ) {}
    Time( double t_value, const string& t_value_unit_given="DEFAULT", const string& t_value_unit_use="DEFAULT",
          double t_error=0, const string& t_error_unit_given="DEFAULT", const string& t_error_unit_use="DEFAULT" ) :
        Measurement( t_value, t_value_unit_given, t_value_unit_use, t_error, t_error_unit_given, t_error_unit_use ) {}
   ~Time() {}

protected:
    inline static map< string, double > m_unitMap = {
        { "DEFAULT", 1e+0  },

        { "ps"     , 1e-12 },
        { "ns"     , 1e-9  },
        { "us"     , 1e-6  },
        { "ms"     , 1e-3  },
        { "s"      , 1e+0  },
        { "ks"     , 1e+3  },
        { "Ms"     , 1e+6  },
        { "Gs"     , 1e+9  },
        { "Ts"     , 1e+12 },
        { "Ps"     , 1e+15 }
    };
};

class Mass : public Measurement< Mass >
{
public:
    Mass() : Measurement() {}
    Mass( const Mass& t_mass ) : Measurement( t_mass ) {}
    Mass( double t_value, const string& t_value_unit_given="DEFAULT", const string& t_value_unit_use="DEFAULT",
          double t_error=0, const string& t_error_unit_given="DEFAULT", const string& t_error_unit_use="DEFAULT" ) :
        Measurement( t_value, t_value_unit_given, t_value_unit_use, t_error, t_error_unit_given, t_error_unit_use ) {}
   ~Mass() {}

protected:
    inline static map< string, double > m_unitMap = {
        { "DEFAULT", 1e+0  },

        { "pg"     , 1e-12 },
        { "ng"     , 1e-9  },
        { "ug"     , 1e-6  },
        { "mg"     , 1e-3  },
        { "g"      , 1e+0  },
        { "kg"     , 1e+3  },
        { "Mg"     , 1e+6  },
        { "Gg"     , 1e+9  },
        { "Tg"     , 1e+12 },
        { "Pg"     , 1e+15 },

        { "oz"     , 0.0283495 },
        { "lb"     , 0.453592  },

        { "ton"    , 907.185   },
        { "tonne"  , 1000      }
    };
};

class Angle : public Measurement< Angle >
{
public:
    Angle() : Measurement() {}
    Angle( const Angle& t_angle ) : Measurement( t_angle ) {}
    Angle( double t_value, const string& t_value_unit_given="DEFAULT", const string& t_value_unit_use="DEFAULT",
           double t_error=0, const string& t_error_unit_given="DEFAULT", const string& t_error_unit_use="DEFAULT" ) :
        Measurement( t_value, t_value_unit_given, t_value_unit_use, t_error, t_error_unit_given, t_error_unit_use ) {}
   ~Angle() {}

protected:
    inline static map< string, double > m_unitMap = {
        { "DEFAULT", 1e+0  },

        { "prad"   , 1e-12 },
        { "nrad"   , 1e-9  },
        { "urad"   , 1e-6  },
        { "mrad"   , 1e-3  },
        { "rad"    , 1e+0  },
        { "krad"   , 1e+3  },
        { "Mrad"   , 1e+6  },
        { "Grad"   , 1e+9  },
        { "Trad"   , 1e+12 },
        { "Prad"   , 1e+15 },

        { "deg"    , pi / 180 },
        { "arcmin" , pi / 10800 },
        { "arcsec" , pi / 648000 }
    };
};

template< typename MeasurementType >
Measurement< MeasurementType >::Measurement( const Measurement& t_measurement ) :
    m_value          { t_measurement.m_value           },
    m_value_unit     { t_measurement.m_value_unit      },
    m_value_unit_name{ t_measurement.m_value_unit_name },
    m_error          { t_measurement.m_error           },
    m_error_unit     { t_measurement.m_error_unit      },
    m_error_unit_name{ t_measurement.m_error_unit_name }
{}

template< typename MeasurementType >
Measurement< MeasurementType >::Measurement( double t_value, const string& t_value_unit_given, const string& t_value_unit_use,
                                             double t_error, const string& t_error_unit_given, const string& t_error_unit_use ) :
    m_value          { t_value },
    m_error          { t_error },
    m_value_unit_name{ t_value_unit_given },
    m_error_unit_name{ t_error_unit_given }
{
    m_value_unit = MeasurementType::m_unitMap.at( t_value_unit_given );
    m_error_unit = MeasurementType::m_unitMap.at( t_error_unit_given );

    update_value( t_value_unit_use );
    update_error( t_error_unit_use );
}

template< typename MeasurementType >
double Measurement< MeasurementType >::get_updated_value( double t_value, const string& t_unit_given, const string& t_unit_use ) const
{
    if( t_unit_given == t_unit_use ) return t_value;

    return t_value * MeasurementType::m_unitMap.at( t_unit_given ) / MeasurementType::m_unitMap.at( t_unit_use );
}

template< typename MeasurementType >
pair< double, string > Measurement< MeasurementType >::get_updated_value_smart( double t_value, const string& t_unit_given, unsigned int t_nMaxDigitsIntegerPart ) const
{
    if( t_nMaxDigitsIntegerPart == 0 ) return pair< double, string >( { t_value, t_unit_given } );

    map< double, string > unitMapSorted;
    for( const auto& unit : MeasurementType::m_unitMap ) 
        unitMapSorted[ unit.second ] = unit.first;
    
    for( const auto& unit : unitMapSorted ) {
        double value = get_updated_value( t_value, t_unit_given, unit.second );
        if( value > pow( 10, t_nMaxDigitsIntegerPart ) ) {
            return pair< double, string >( { value, unit.second } );
        }
    }

    return pair< double, string >( { t_value, t_unit_given } );
}

template< typename MeasurementType >
MeasurementType& Measurement< MeasurementType >::update_value( double t_value, const string& t_value_unit )
{
    if( m_value_unit_name == t_value_unit ) {
        m_value = t_value;
        return *this;
    }

    m_value = get_updated_value( t_value, t_value_unit );
    m_value_unit = MeasurementType::m_unitMap.at( t_value_unit );
    m_value_unit_name = t_value_unit;

    return *this;
}

template< typename MeasurementType >
MeasurementType& Measurement< MeasurementType >::update_error( double t_error, const string& t_error_unit )
{
    if( m_error_unit_name == t_error_unit ) {
        m_error = t_error;
        return *this;
    }

    m_error = get_updated_error( t_error, t_error_unit );
    m_error_unit = MeasurementType::m_unitMap.at( t_error_unit );
    m_error_unit_name = t_error_unit;

    return *this;
}

template< typename MeasurementType >
bool Measurement< MeasurementType >::operator()( const MeasurementType& t_measurement_0, const MeasurementType& t_measurement_1, double t_epsilon ) const
{
    return abs( t_measurement_0.get_value_smart() - t_measurement_1.get_value_smart() ) < t_epsilon ? t_measurement_0 < t_measurement_1 : false;
}

#endif // MEASUREMENTS_H