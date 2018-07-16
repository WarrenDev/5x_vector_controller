
/**
* gatt_uuid.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  GATT UUID types.
*
*/

#ifndef GATT_UUID_H
#define GATT_UUID_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * WARNING: The 16-bit UUIDs are assigned by the Bluetooth SIG and published
 *          in the Bluetooth Assigned Numbers page. Do not change these values.
 *          Changing them will cause Bluetooth interoperability issues.
 */

/**
 * GATT Services
 */
#define GAP_SERVICE_UUID                           0x1800 // Generic Access Profile
#define GATT_SERVICE_UUID                          0x1801 // Generic Attribute Profile

/**
 * GATT Declarations
 */
#define GATT_PRIMARY_SERVICE_UUID                  0x2800 // Primary Service
#define GATT_SECONDARY_SERVICE_UUID                0x2801 // Secondary Service
#define GATT_INCLUDE_UUID                          0x2802 // Include
#define GATT_CHARACTER_UUID                        0x2803 // Characteristic

/**
 * GATT Descriptors
 */
#define GATT_CHAR_EXT_PROPS_UUID                   0x2900 // Characteristic Extended Properties
#define GATT_CHAR_USER_DESC_UUID                   0x2901 // Characteristic User Description
#define GATT_CLIENT_CHAR_CFG_UUID                  0x2902 // Client Characteristic Configuration
#define GATT_SERV_CHAR_CFG_UUID                    0x2903 // Server Characteristic Configuration
#define GATT_CHAR_FORMAT_UUID                      0x2904 // Characteristic Format
#define GATT_CHAR_AGG_FORMAT_UUID                  0x2905 // Characteristic Aggregate Format
#define GATT_VALID_RANGE_UUID                      0x2906 // Valid Range
#define GATT_EXT_REPORT_REF_UUID                   0x2907 // External Report Reference Descriptor
#define GATT_REPORT_REF_UUID                       0x2908 // Report Reference Descriptor

/**
 * GATT Characteristics
 */
#define DEVICE_NAME_UUID                           0x2A00 // Device Name
#define APPEARANCE_UUID                            0x2A01 // Appearance
#define PERI_PRIVACY_FLAG_UUID                     0x2A02 // Peripheral Privacy Flag
#define RECONNECT_ADDR_UUID                        0x2A03 // Reconnection Address
#define PERI_CONN_PARAM_UUID                       0x2A04 // Peripheral Preferred Connection Parameters
#define SERVICE_CHANGED_UUID                       0x2A05 // Service Changed

/**
 * GATT Units
 */
#define GATT_UNITLESS_UUID                         0x2700 // <Symbol>, <Expressed in terms of SI base units>
#define GATT_UNIT_LENGTH_METER_UUID                0x2701 // m, m
#define GATT_UNIT_MASS_KGRAM_UUID                  0x2702 // kg, kg
#define GATT_UNIT_TIME_SECOND_UUID                 0x2703 // s, s
#define GATT_UNIT_ELECTRIC_CURRENT_A_UUID          0x2704 // A, A
#define GATT_UNIT_THERMODYNAMIC_TEMP_K_UUID        0x2705 // K, K
#define GATT_UNIT_AMOUNT_SUBSTANCE_M_UUID          0x2706 // mol, mol
#define GATT_UNIT_LUMINOUS_INTENSITY_C_UUID        0x2707 // cd, cd

#define GATT_UNIT_AREA_SQ_MTR_UUID                 0x2710 // m^2, m^2
#define GATT_UNIT_VOLUME_CUBIC_MTR_UUID            0x2711 // m^3, m^3
#define GATT_UNIT_VELOCITY_MPS_UUID                0x2712 // m/s, m s^-1
#define GATT_UNIT_ACCELERATION_MPS_SQ_UUID         0x2713 // m/s^2, m s^-2
#define GATT_UNIT_WAVENUMBER_RM_UUID               0x2714 // ó, m^-1
#define GATT_UNIT_DENSITY_KGPCM_UUID               0x2715 // p, kg m^-3
#define GATT_UNIT_SURFACE_DENSITY_KGPSM_UUID       0x2716 // pA, kg m^-2
#define GATT_UNIT_SPECIFIC_VOLUME_CMPKG_UUID       0x2717 // v, m^3 kg^-1
#define GATT_UNIT_CURRENT_DENSITY_APSM_UUID        0x2718 // j, A m^-2
#define GATT_UNIT_MAGNETIC_FIELD_STRENGTH_UUID     0x2719 // H, A m
#define GATT_UNIT_AMOUNT_CONCENTRATE_MPCM_UUID     0x271A // c, mol m^-3
#define GATT_UNIT_MASS_CONCENTRATE_KGPCM_UUID      0x271B // c, kg m^-3
#define GATT_UNIT_LUMINANCE_CPSM_UUID              0x271C // Lv, cd m^-2
#define GATT_UNIT_REFRACTIVE_INDEX_UUID            0x271D // n, 1
#define GATT_UNIT_RELATIVE_PERMEABLILTY_UUID       0x271E // u, 1
#define GATT_UNIT_PLANE_ANGLE_RAD_UUID             0x2720 // rad, m m-1
#define GATT_UNIT_SOLID_ANGLE_STERAD_UUID          0x2721 // sr, m2 m-2
#define GATT_UNIT_FREQUENCY_HTZ_UUID               0x2722 // Hz, s-1
#define GATT_UNIT_FORCE_NEWTON_UUID                0x2723 // N, m kg s-2
#define GATT_UNIT_PRESSURE_PASCAL_UUID             0x2724 // Pa, N/m2 = m2 kg s-2
#define GATT_UNIT_ENERGY_JOULE_UUID                0x2725 // J, N m = m2 kg s-2
#define GATT_UNIT_POWER_WATT_UUID                  0x2726 // W, J/s = m2 kg s-3
#define GATT_UNIT_ELECTRIC_CHARGE_C_UUID           0x2727 // C, sA
#define GATT_UNIT_ELECTRIC_POTENTIAL_DIF_V_UUID    0x2728 // V, W/A = m2 kg s-3 A-1

#define GATT_UNIT_CELSIUS_TEMP_DC_UUID             0x272F // oC, t/oC = T/K - 273.15

#define GATT_UNIT_TIME_MINUTE_UUID                 0x2760 // min, 60 s
#define GATT_UNIT_TIME_HOUR_UUID                   0x2761 // h, 3600 s
#define GATT_UNIT_TIME_DAY_UUID                    0x2762 // d, 86400 s
#define GATT_UNIT_PLANE_ANGLE_DEGREE_UUID          0x2763 // o, (pi/180) rad
#define GATT_UNIT_PLANE_ANGLE_MINUTE_UUID          0x2764 // ', (pi/10800) rad
#define GATT_UNIT_PLANE_ANGLE_SECOND_UUID          0x2765 // '', (pi/648000) rad
#define GATT_UNIT_AREA_HECTARE_UUID                0x2766 // ha, 10^4 m^2
#define GATT_UNIT_VOLUME_LITRE_UUID                0x2767 // l, 10^-3 m^3
#define GATT_UNIT_MASS_TONNE_UUID                  0x2768 // t, 10^3 kg

#define GATT_UINT_LENGTH_YARD_UUID                 0x27A0 // yd, 0.9144 m
#define GATT_UNIT_LENGTH_PARSEC_UUID               0x27A1 // pc, 3.085678 × 1016 m
#define GATT_UNIT_LENGTH_INCH_UUID                 0x27A2 // in, 0.0254 m
#define GATT_UNIT_LENGTH_FOOT_UUID                 0x27A3 // ft, 0.3048 m
#define GATT_UNIT_LENGTH_MILE_UUID                 0x27A4 // mi, 1609.347 m
#define GATT_UNIT_PRESSURE_PFPSI_UUID              0x27A5 // psi, 6.894757 × 103 Pa
#define GATT_UNIT_VELOCITY_KMPH_UUID               0x27A6 // km/h, 0.2777778 m^s-1
#define GATT_UNIT_VELOCITY_MPH_UUID                0x27A7 // mi/h, 0.44704 m^ s-1
#define GATT_UNIT_ANGULAR_VELOCITY_RPM_UUID        0x27A8 // r/min, 0.1047198 rad s-1
#define GATT_UNIT_ENERGY_GCAL_UUID                 0x27A9 //
#define GATT_UNIT_ENERGY_KCAL_UUID                 0x27AA // kcal, 4190.02 J
#define GATT_UNIT_ENERGY_KWH_UUID                  0x27AB // kWh, 3600000 J
#define GATT_UNIT_THERMODYNAMIC_TEMP_DF_UUID       0x27AC // oF, t/oF = T/K × 1.8 - 459.67
#define GATT_UNIT_PERCENTAGE_UUID                  0x27AD // %
#define GATT_UNIT_PER_MILE_UUID                    0x27AE //
#define GATT_UNIT_PERIOD_BPM_UUID                  0x27AF //
#define GATT_UNIT_ELECTRIC_CHARGE_AH_UUID          0x27B0 //
#define GATT_UNIT_MASS_DENSITY_MGPD_UUID           0x27B1 //
#define GATT_UNIT_MASS_DENSITY_MMPL_UUID           0x27B2 //
#define GATT_UNIT_TIME_YEAR_UUID                   0x27B3 //
#define GATT_UNIT_TIME_MONTH_UUID                  0x27B4 //


/**
 * GATT Services
 */
extern const uint8_t gapServiceUUID[];
extern const uint8_t gattServiceUUID[];

/**
 * GATT Attribute Types
 */
extern const uint8_t primaryServiceUUID[];
extern const uint8_t secondaryServiceUUID[];
extern const uint8_t includeUUID[];
extern const uint8_t characterUUID[];

/**
 * GATT Characteristic Descriptors
 */
extern const uint8_t charExtPropsUUID[];
extern const uint8_t charUserDescUUID[];
extern const uint8_t clientCharCfgUUID[];
extern const uint8_t servCharCfgUUID[];
extern const uint8_t charFormatUUID[];
extern const uint8_t charAggFormatUUID[];
extern const uint8_t validRangeUUID[];
extern const uint8_t extReportRefUUID[];
extern const uint8_t reportRefUUID[];

/**
 * GATT Characteristic Types
 */
extern const uint8_t deviceNameUUID[];
extern const uint8_t appearanceUUID[];
extern const uint8_t periPrivacyFlagUUID[];
extern const uint8_t reconnectAddrUUID[];
extern const uint8_t periConnParamUUID[];
extern const uint8_t serviceChangedUUID[];

#ifdef __cplusplus
}
#endif

#endif /* GATT_UUID_H */
