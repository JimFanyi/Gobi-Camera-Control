///
/// @file XFooters.h
/// The header file for the per frame footer structure definitions
///
#ifndef _X_FOOTERS_H_
#   define _X_FOOTERS_H_


#pragma pack(push, 1)

    ///
    /// @brief Hardware footer structure for ONCA class cameras (class PID = 0xf040)
    ///
    typedef struct
    {
        union
        {
            struct
            {
                unsigned short trig_ext:1;          ///< External trigger state
                unsigned short trig_cl:1;           ///< Camera link trigger pin state
                unsigned short trig_soft:1;         ///< Software trigger state
                unsigned short reserved:5;          ///< RFU
                unsigned short linecam_fixedSH:1;   /// line camera: uses single readout
                unsigned short linecam_SHBfirst:1;  /// line camera: order of the lines
                unsigned short reserved2:3;         ///< RFU
                unsigned short filterwheel:3;       ///< Current filter wheel position
            } statusbits;
            unsigned short field;
        } status;
        unsigned int   tint;                        ///< Active exposure time in truncated us.
        unsigned int   timelo;                      ///< Timestamp lo
        unsigned int   timehi;                      ///< Timestamp hi (64-bit since the start of the unix epoch)
        unsigned short temp_die;                    ///< Die temperature in degrees Kelvin
        unsigned short temp_case;                   ///< Case temperature in degrees Kelvin
    } XPFF_F040;

    ///
    /// @brief Hardware footer structure for GOBI class cameras (class PID = 0xf003)
    ///
    typedef struct
    {
        union
        {
            struct
            {
                unsigned short trig_ext  :1;       ///<External trigger state
                unsigned short reserved  :15;
            } statusbits;
            unsigned short field;
        } status;
        unsigned int    tint;                      ///< Integration time in microseconds
        unsigned int    timelo;                    ///< Time stamp low
        unsigned int    timehi;                    ///< Time stamp hi (64-bit integer since the start of the Unix epoch)
        unsigned short  temp_die;                  ///< Sensor temperature (Die temp) in centiKelvin
        unsigned short  reserved1;
        unsigned short  tag;
        unsigned int    image_offset;              ///<Global offset applied to all pixels in the frame (signed 32 bit number)
        unsigned short  image_gain;                ///<Global gain applied to the pixels in the frame (8.8 fixed point number)
        unsigned int    reserved2;
    } XPFF_F003;

    ///
    /// @brief Per frame footer combining both the software and hardware footers in one structure.
    /// @warning Use XCamera::GetFrameFooterLength() to learn the exact size to allocate for the footer data.
    ///
    typedef struct
    {
        unsigned short len;     ///< Structure length
        unsigned short ver;     ///< Version (0xAA00)

        long long      soc;     ///< Time of Start Capture
        long long      tft;     ///< Time of reception
        dword          tfc;     ///< Frame counter
        dword          fltref;  ///< Filter marker, top nibble specifies purpose\n
                                ///< 0x1xxxxxxx - Filter generated trigger event (x = filter specific)\n
                                ///< 0x2xxxxxxx - Start / end of sub-sequence marker (x = 0 / x = 1)
        dword          hfl;     ///< Hardware footer length

        ///
        /// @brief Unified hardware footers
        /// Use the footer class identifier field to differentiate between the differentiate footer classes.
        ///
        struct
        {
            unsigned short pid;              ///< Footer class identifier
            union
            {
                XPFF_F040      onca;         ///< PID == 0xF040
                XPFF_F003      gobi;         ///< PID == 0xF020, 0xF021, 0xF031
            } Cameras;
        } Common;

    } XPFF_GENERIC;

#pragma pack(pop)

#endif