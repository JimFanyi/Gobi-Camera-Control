///
/// @file XCamera.h 
/// @brief This file contains the camera interface definitions.
///
/// @note This header file is documented using the Doxygen documentation framework.
/// On Windows systems refer to the xeneth-sdk.chm file found in the Help-folder 
/// which is located in the SDK installation directory (Default: \%ProgramFiles\%\\Xeneth\\Sdk)
/// For Linux systems refer to /usr/share/doc/xeneth-major-minor-subminor/index.html.
///
#ifndef _XENICS_XCAMERA_GEN2_H_
#   define _XENICS_XCAMERA_GEN2_H_

#ifdef _WIN32
#   ifndef CALLCV
#       define CALLCV       __stdcall                       ///< Compatibility with VB6
#       define CALLCVCB     __cdecl
#   else
#       pragma message ("Warning: building using custom calling convention.")
#   endif

#   ifdef _USRDLL
#       define IMPEXP   __declspec(dllexport)               ///< Attributes for C++ exports
#       define IMPEXPC  __declspec(dllexport) CALLCV        ///< Attributes for C   exports
#   else
#       define IMPEXP   __declspec(dllimport)               ///< Attributes for C++ exports
#       define IMPEXPC  __declspec(dllimport) CALLCV        ///< Attributes for C   exports
#   endif
#else
#   define CALLCV
#   define CALLCVCB
#   define IMPEXP
#   define IMPEXPC
#endif

#ifdef __cplusplus
    extern "C" {
#endif

    ///
    /// @brief These are the error codes used throughout the API.
    /// To get a string representation of the error code at runtime make sure to check the XC_ErrorToString() function.
    ///
    typedef enum
    {
        I_OK                    = 0,        ///< Success.
        I_DIRTY                 = 1,        ///< Internal.
        E_BUG                   = 10000,    ///< Generic.
        E_NOINIT                = 10001,    ///< Camera was not successfully initialised.
        E_LOGICLOADFAILED       = 10002,    ///< Invalid logic file.
        E_INTERFACE_ERROR       = 10003,    ///< Command interface failure.
        E_OUT_OF_RANGE          = 10004,    ///< Provided value is incapable of being produced by the hardware.
        E_NOT_SUPPORTED         = 10005,    ///< Functionality not supported by this camera.
        E_NOT_FOUND             = 10006,    ///< File/Data not found.
        E_FILTER_DONE           = 10007,    ///< Filter has finished processing, and will be removed.
        E_NO_FRAME              = 10008,    ///< A frame was requested by calling GetFrame, but none was available.
        E_SAVE_ERROR            = 10009,    ///< Couldn't save to file.
        E_MISMATCHED            = 10010,    ///< Buffer size mismatch.
        E_BUSY                  = 10011,    ///< The API can not read a temperature because the camera is busy.
        E_INVALID_HANDLE        = 10012,    ///< An unknown handle was passed to the C API.
        E_TIMEOUT               = 10013,    ///< Operation timed out.
        E_FRAMEGRABBER          = 10014,    ///< Frame grabber error.
        E_NO_CONVERSION         = 10015,    ///< GetFrame could not convert the image data to the requested format.
        E_FILTER_SKIP_FRAME     = 10016,    ///< Filter indicates the frame should be skipped.
        E_WRONG_VERSION         = 10017,    ///< Version mismatch.
        E_PACKET_ERROR          = 10018,    ///< The requested frame cannot be provided because at least one packet has been lost.
        E_WRONG_FORMAT          = 10019,    ///< The emissivity map you tried to set should be a 16 bit greyscale PNG.
        E_WRONG_SIZE            = 10020,    ///< The emissivity map you tried to set has the wrong dimensions (width, height).
        E_CAPSTOP               = 10021,    ///< Internal
        E_OUT_OF_MEMORY         = 10022,    ///< An allocation failed because the system ran out of memory.
        E_RFU
    } ErrorCodes;

    ///
    /// @brief The different colour mappings available for greyscale data sources.
    /// These modes affects how colour coded images are displayed and saved to disk.
    /// These flags are used by XCamera::SetColourMode and XCamera::GetColourMode.
    ///
    typedef enum
    {
        ColourMode_8            = 0,    ///< Intensity only
        ColourMode_16           = 1,    ///< Alias
        ColourMode_Profile      = 2,    ///< Uses a colour profile bitmap. See #LoadColourProfile()
        ColourMode_Invert       = 256   ///< Set this flag if an inversion of the colour profile is desired. eg: #ColourMode_8 | #ColourMode_Invert
    } ColourMode;

    ///
    /// @brief The different destinations supported by the XCamera::Blit-method.
    ///
    typedef enum
    {
        Window          = 0,    /**< Blit the contents of the last captured frame directly to a Windows client device context using a Window handle (HWND) */
        DeviceContext   = 1     /**< Blit the contents of the last captured frame to a specified device context.
                                     This can be any device context (HDC) like a memory DC, paint DC or a handle to a DC associated with a Graphics-object (C#)   */
    } BlitType;

    ///
    /// @brief The supported frame types
    ///
    /// These are used to learn the native pixel size of the camera using XCamera::GetFrameType,
    /// or during frame conversions when calling the XCamera::GetFrame(FrameType, unsigned long, void *, unsigned int)-method.
    ///
    typedef enum
    {
        FT_UNKNOWN      = -1,   ///< Unknown invalid frame type
        FT_NATIVE       = 0,    ///< The native frame type of this camera (can be FT_8..,FT_16..,FT32.. check GetFrameType())
        FT_8_BPP_GRAY   = 1,    ///< 8-bit greyscale
        FT_16_BPP_GRAY  = 2,    ///< 16-bit greyscale (default for most of the Xenics branded cameras)
        FT_32_BPP_GRAY  = 3,    ///< 32-bit greyscale
        FT_32_BPP_RGBA  = 4,    ///< 32-bit colour RGBA      [B,G,R,A] Available for output conversion.
        FT_32_BPP_RGB   = 5,    ///< 32-bit colour RGB       [B,G,R]   Available for output conversion.
        FT_32_BPP_BGRA  = 6,    ///< 32-bit colour BGRA      [R,G,B,A]
        FT_32_BPP_BGR   = 7     ///< 32-bit colour BGR       [R,G,B]
    } FrameType;

    /* Basic types */

    typedef int            XCHANDLE;    ///< Handle type used to identify an initialised session to a camera.
    typedef unsigned long  ErrCode;     ///< Type to hold an error code (see #ErrorCodes)
    typedef unsigned long  FilterID;    ///< Filter ID handle type used to identify a filter registered to the filter stack of a #XCamera-object.

#if defined(__LP64__)   // Handle systems with 8-byte "long" types (such as Linux 64bit) without altering the .dll/.so signature on pre-existing systems.
    typedef unsigned int   dword;       ///< 4 bytes (32-bits)
#else
    typedef unsigned long  dword;       ///< 4 bytes (32-bits)
#endif
    typedef unsigned short word;        ///< 2 bytes (16-bits)
    typedef unsigned char  byte;        ///< 1 byte (8-bits)
    typedef unsigned char  boole;       ///< 1 byte (8-bits)
    typedef void *         voidp;       ///< void pointer type

    ///
    /// @brief Image filter messages
    ///
    typedef enum
    {
        XMsgInit                = 0,    /**< [API->Filter Event] Called when the filter is being installed  ( (!) calling thread context)         */
        XMsgClose               = 1,    /**< [API->Filter Event] Called when the filter is being removed    ( (!) calling thread context)         */
        XMsgFrame               = 2,    /**< [API->Filter Event] Called after every frame grab              ( (!) grabbing thread context)        */
        XMsgGetName             = 3,    /**< [App->Filter Event] Retrieve filter name: the filter should copy a friendly string to msgparm        */
        XMsgGetValue            = 4,    /**< [Obsolete]                                                                                           */
        XMsgSave                = 5,    /**< [Obsolete]                                                                                           */
        XMsgGetStatus           = 6,    /**< [API->Filter Event] Retrieves a general purpose status message from the image filter                 */
        XMsgUpdateViewPort      = 7,    /**< [API->Filter Event] Instructs an image correction filter to update it's view port
                                                                 This message is sent to a filter upon changing the window of interest, or when
                                                                 flipping image horizontally or vertically                                        */
        XMsgCanProceed          = 8,    /**< Used by image filters in in interactive mode to indicate acceptable image conditions                 */
        XMsgGetInfo             = 9,    /**< [Internal]          Used to query filter 'registers'                                                 */
        XMsgSelect              = 10,   /**< [Obsolete]                                                                                           */
        XMsgProcessedFrame      = 11,   /**< [API->Filter Event] Sent after other filters have done their processing. Do not modify the frame data
                                                                 in response to this event.                                                       */
        XMsgTimeout             = 13,   /**< [API->Filter Event] A camera time-out event was generated                                             */
        XMsgIsBusy              = 16,   /**< [Thermography]      Is the temperature filter recalculating - Used to check if the thermal filter is
                                                                 still updating it's linearisation tables                                         */
        XMsgSetTROI             = 17,   /**< [Imaging/Thermo]    Set the adu/temperature span in percent, (see #XMsgSetTROIParms)                 */
        XMsgLoad                = 18,   /**< [Obsolete]                                                                                           */
        XMsgUnload              = 19,   /**< [Obsolete]                                                                                           */
        XMsgADUToTemp           = 12,   /**< [Thermography]      Convert an ADU value to a temperature (see #XFltADUToTemperature)                 */
        XMsgGetEN               = 14,   /**< [Obsolete]          Get temperature correction parameters (see #XMsgGetRadiometricParms)              */
        XMsgSetEN               = 15,   /**< [Obsolete]          Set temperature correction parameters (see #XMsgGetRadiometricParms)              */
        XMsgTempToADU           = 20,   /**< [Thermography]      Convert a temperature to an ADU value (see #XFltTemperatureToADU)                 */
        XMsgGetTValue           = 21,   /**< [Thermography]      Retrieve an emissivity corrected value from a coordinate                         */
        XMsgGetRadiometricParms = 22,   /**< [Thermography]      Get temperature correction parameters (see #XMsgTempParms)                        */
        XMsgSetRadiometricParms = 23,   /**< [Thermography]      Set temperature correction parameters (see #XMsgTempParms)                        */
        XMsgSerialise           = 100,  /**< [App->Filter event] Serialise internal parameter state (write xml structure) see #XFltSetParameter    */
        XMsgDeserialise         = 101,  /**< [App->Filter event] Deserialise parameter state (read xml structure) see #XFltSetParameter            */
        XMsgGetPriority         = 102,  /**< [Filter Management] Write the current filter priority to the long * provided in v_pMsgParm           */
        XMsgSetFilterState      = 104,  /**< [Filter Management] Enable or disable an image filter temporarily by sending 0/1 in v_pMsgParm       */
        XMsgIsSerialiseDirty    = 105,  /**< [Internal]                                                                                           */
        XMsgStoreHandle         = 106,  /**< [Internal]          Start tracking the module handle for plugin image filters                        */
        XMsgUpdateTint          = 107,  /**< [API->Filter event] Integration time change notification                                             */
        XMsgLinADUToTemp        = 109,  /**< [Thermography]      Convert a Linearized ADU value to a temperature (see #XFltADUToTemperatureLin)    */
        XMsgLinTempToADU        = 110,  /**< [Thermography]      Convert a temperature to a Linearized ADU value (see #XFltTemperatureToADULin)    */
        XMsgUpdateSpan          = 111,  /**< [API->Filter event] Span change notification                                                         */
        XMsgUpdatePalette       = 112,  /**< [API->Filter event] Colour profile change notification                                               */
        XMsgFilterQueued        = 113,  /**< [API->Filter event] A filter is queued                                                               */
        XMsgFilterRemoved       = 114,  /**< [API->Filter event] A filter is removed                                                              */
        XMsgDrawOverlay         = 200,  /**< [API->Filter event] Draw the RGBA frame overlay, v_pMsgParm is the pointer to the RGBA data
                                                                 structure                                                                        */
        XMsgLineariseOutput     = 201,  /**< [Thermography]      When specifying a v_pMsgParm that is non zero, starts linearising adu output     */
        XMsgSetEmiMap           = 202,  /**< [Thermography]      Streams the main emissivity map to the thermal filter (16 bit png, 65535 = 1.0)  */
        XMsgSetEmiMapUser       = 203,  /**< [Thermography]      Stream a user emissivity map to the thermal filter (16 bit png, 65535 = 1.0,
                                                                 0 values are replaced by the emissivity in the main map)                         */
        XMsgGetEmiMap           = 204,  /**< [Thermography]      Stream out the combined emissivity map                                           */
        XMsgClrEmiMap           = 205,  /**< [Thermography]      Clear emissivity map                                                             */
        XMsgClrEmiMapUser       = 206,  /**< [Thermography]      Clear emissivity map (user)                                                      */
        XMsgPushRange           = 207,  /**< [Thermography]      Push a new linearization range to the thermal filter                             */
        XMsgThmFilterState      = 208,  /**< [Thermography]      Filter event indicating thermal filter queue/removal                             */
        XMsgThmAdjustSet        = 209,  /**< [Thermography]      Set global offset & gain adu adjustment (pre-thermal conversion)                 */
        XMsgThmAdjustGet        = 210,  /**< [Thermography]      (see #XMsgTempAdjustmentParms)                                                      */

        XMsgLog                 = 211,  /**< [Plugin->API]       Fire a log event to the end user application\n
                                                                 Target filter id: 0xffffffff                                                */
        XMsgGetDeltaT           = 212,  /**< [Internal]                                                                                      */
        XMsgGetTintRange        = 213,  /**< [Plugin->API]       Request the exposure time range
                                                                 Target filter id: 0xffffffff                                                 */
        XMsgCorrectionDirty         = 214,  /**< [Internal]          The onboard thermography parameters have changed                             */
        XMsgHasRadianceInfo         = 215,  /**< [Thermography]      Check if the radiance information is available. This is needed to for emissivity correction */
        XMsgCorrectionDataChanged   = 216,  /**< [Internal]          New correction data is loaded                             */

        XMsgZoomLensConnect     = 300,  /**< [Zoom lens]         Connect to the zoom lens on the specified port.  */
        XMsgZoomLensGetState    = 301,  /**< [Zoom lens]         Get the current zoom/focus state from the zoom lens filter.  */
        XMsgZoomLensSetState    = 302,  /**< [Zoom lens]         Set the current zoom/focus state in the zoom lens filter.    */
        XMsgZoomLensGetInfo     = 303,  /**< [Zoom lens]         Get some descriptive information about the connected lens.   */

        XMsgUser                = 24200 /**< If you develop your own image filter plugins, please use this constant to offset your messages. */
    } XFilterMessage;

    ///
    /// @brief Image filter callback prototype.
    ///
    /// @param[in]  v_pCamera       pointer to camera object
    /// @param[in]  v_pUserParm     user argument as specified in XCamera::AddImageFilter
    /// @param[in]  tMsg            as per #XFilterMessage
    /// @param[in]  v_pMsgParm      as per XCamera::MsgImageFilter
    ///
    /// @return The user has to return #I_OK on success.
    ///         On failure one of the other possible values should be returned.
    ///
    /// @retval #I_OK   Success.
    ///
    typedef ErrCode (CALLCVCB *XImageFilter)(void * v_pCamera, void * v_pUserParm, XFilterMessage tMsg, void * v_pMsgParm);


    ///
    /// @brief Status messages used in conjunction with the #XStatus-callback.
    ///
    typedef enum  {
        XSLoadLogic         = 1,    /**< Passed when loading the camera's main logic file                           */
        XSLoadVideoLogic    = 2,    /**< Passed when loading the camera's video output firmware                     */
        XSDataStorage       = 3,    /**< Passed when accessing persistent data on the camera                        */
        XSCorrection        = 4,    /**< Passed when uploading correction data to the camera                        */
        XSSelfStart         = 5,    /**< Passed when a self starting camera is starting (instead of XSLoadLogic)    */
        XSMessage           = 6,    /**< String event
                                     **  This status message is used to relay critical errors, and events originating
                                     **  from within the API.
                                     **  Cam|PropLimit|property=number - A filter notifies you your user interface should limit the value of 'property' to 'number'
                                     **  Cam|TemperatureFilter|RangeUpdate       - The thermography filter uses this to notify you of a span update.
                                     **  Cam|TemperatureFilter|Off               - The thermography filter suggests the application to dequeue the filter.
                                     **  Cam|InterfaceUpdate           - Internal, do not handle, returning E_BUG here causes the API to stop unpacking 'abcd.package'.packages to %appdata%/xenics/interface
                                     **
                                     **
                                     **/
        XSLoadGrabber       = 7,    /**< Passed when loading the framegrabber                                                                                */
        XSDeviceInformation = 8     /**< Device information passed when connecting a device, ulP is the lower part of the address. When using 64-bit the higher part of the address is stored in ulT */
    } XStatusMessage;


    ///
    /// @brief Status callback prototype.
    ///
    /// This function is called by the SDK to report camera and interface events,
    /// indicate progress while saving or loading blobs and general messages.
    /// When a status function is declared the user is able to register this callback by
    /// passing a pointer to the function as the pCallBack-argument when instantiating
    /// a new XCamera-object (XC_OpenCamera() or XCamera::Create()).
    ///
    /// @code
    /// #include "XCamera.h"
    /// #include <stdio.h>
    /// 
    /// ErrCode XStatusCallback(void *v_pUserParm, int iMsg, unsigned long ulP, unsigned long ulT) {
    ///     if (iMsg == XSMessage) {
    ///
    /// #ifdef _WIN64
    ///         char * string_buffer = (char *)(ulP | (unsigned long long)ulT << 32);
    /// #else
    ///         char * string_buffer = (char *)ulP;
    /// #endif
    ///
    ///         printf("[MESSAGE] %s\r\n", string_buffer);
    /// 
    ///     }
    ///     else if (iMsg == XSDeviceInformation) {
    ///
    /// #ifdef _WIN64
    ///         XDeviceInformation * device_info = (XDeviceInformation *)(ulP | (unsigned long long)ulT << 32);
    /// #else
    ///         XDeviceInformation * device_info = (XDeviceInformation *)ulP;
    /// #endif
    ///
    ///         printf("[DEVINFO] %s (%i)\r\n", device_info->name, device_info->serial);
    ///
    ///     }
    ///     else {
    ///         printf("[UNHANDLED] iMsg = %i\r\n", iMsg);
    ///     }
    ///
    ///     return I_OK;
    /// }
    ///
    /// int main() {    
    ///     // ...    
    ///     XCHANDLE handle = XC_OpenCamera("soft://0", XStatusCallback, 0);    
    ///     // ...    
    ///     return 0;    
    /// }
    /// @endcode
    ///
    /// <b>SDK messages:</b>
    /// When iMsg is set to #XSMessage the user can obtain a string buffer from the SDK.
    /// This buffer will contain messages such as Informative event messages, warnings
    /// or error messages that can be useful when debugging the user code.
    /// To obtain this buffer convert ulP (32-bit), or both ulP and ulT combined (64-bit) to
    /// a char-pointer where ulP is the low part of the address and for 64-bit ulT is used
    /// to represent the high part of the address.
    ///
    /// <b>Device information:</b>
    /// The device information messages work in a same manner as the SDK messages. In case when iMsg is 
    /// set to #XSDeviceInformation the user can obtain the #XDeviceInformation-structure.
    /// To obtain this structure convert ulP (32-bit), or both ulP and ulT combined (64-bit) to
    /// a XDeviceInformation-pointer where ulP is the low part of the address and for 64-bit ulT is used
    /// to represent the high part of the address.
    ///
    /// @param[in]  v_pUserParm  User argument that will be passed for each call.
    /// @param[in]  iMsg         #XStatusMessage
    /// @param[in]  ulP          Progress or low address depending on the message type.
    /// @param[in]  ulT          Total or high address (only in 64-bit) depending on the message.
    ///
    /// @return The user has to return #I_OK on success.
    ///         On failure one of the other possible values should be returned.
    ///
    /// @retval #I_OK   Success.
    ///
    typedef ErrCode (CALLCVCB *XStatus)(void *v_pUserParm, int iMsg, unsigned long ulP, unsigned long ulT);

    ///
    /// @brief The XCamera::GetFrame(FrameType, unsigned long, void *, unsigned int)-flags
    ///
    /// These flags are used to control the way the GetFrame-method works.
    ///
    typedef enum
    {
        XGF_Blocking    = 1,    ///< In blocking-mode the method does not return immediately with the return codes #E_NO_FRAME / #I_OK.
                                ///  Instead the method waits for a frame and only returns until a frame was captured, or a time-out period has elapsed.

        XGF_NoConversion= 2,    ///< Prevents internal conversion to 8 bit, specifying this flag reduces computation time, but prevents #SaveData() and the #Blit() method from working.
        XGF_FetchPFF    = 4,    ///< Retrieve the per frame footer with frame timing information. Call XCamera::GetFrameFooterLength() to determine the increase in frame size.
        XGF_RFU_1       = 8,
        XGF_RFU_2       = 16,
        XGF_RFU_3       = 32
    } XGetFrameFlags;

    ///
    /// @brief XCamera::SaveData()-flags
    ///
    typedef enum
    {
        XSD_Force16             = 1,    ///< Forces 16-bit output independent of the current #ColourMode-setting (only possible for PNG's)
        XSD_Force8              = 2,    ///< Forces 8-bit output independent of the current #ColourMode
        XSD_AlignLeft           = 4,    ///< Left aligns 16-bit output (#XSD_Force16 | #XSD_AlignLeft)
        XSD_SaveThermalInfo     = 8,    ///< Save thermal conversion structure (only available when saving 16-bit PNGs)
        XSD_RFU_0               = 16,   ///< Reserved
        XSD_RFU_1               = 32,
        XSD_RFU_2               = 64,
        XSD_RFU_3               = 128
    } XSaveDataFlags;

    ///
    /// @brief XCamera::SaveSettings()-flags
    ///
    typedef enum
    {
        XSS_SaveCameraProps     = 1,    ///< Define property sources to save settings from.
        XSS_SaveGrabberProps    = 2,    ///<

        XSS_SaveAllProps        = 4,    ///< Also save properties marked 'No persist'.
        XSS_SS_RFU_3            = 8     ///<
    } XSaveSettingsFlags;

    ///
    /// @brief XCamera::LoadSettings()-flags
    ///
    typedef enum
    {
        XSS_IgnoreNAIS        = 1,    ///< Ignore properties which do not affect the image.
        XSS_LS_RFU_1          = 2,    ///<
        XSS_LS_RFU_2          = 4,    ///<
        XSS_LS_RFU_3          = 8     ///<
    } XLoadSettingsFlags;


    ///
    /// @brief XCamera::LoadCalibration()-flags
    ///
    typedef enum
    {
        XLC_StartSoftwareCorrection     = 1,    ///< Starts the software correction filter after unpacking the calibration data
        XLC_RFU_1                       = 2,
        XLC_RFU_2                       = 4,
        XLC_RFU_3                       = 8
    } XLoadCalibrationFlags;


    ///
    /// @brief Xeneth property types
    ///
    /// These types and their attributes are used by the property system.
    /// To retrieve the property type call the XCamera::GetPropertyType-method using the short name to identify the property.
    ///
    typedef enum
    {
        XType_None              = 0x00000000,

        XType_Base_Mask         = 0x000000ff,   ///< Type mask
        XType_Attr_Mask         = 0xffffff00,   ///< Attribute mask
        XType_Base_Number       = 0x00000001,   ///< A number (floating)
        XType_Base_Enum         = 0x00000002,   ///< An enumerated type (a choice)
        XType_Base_Bool         = 0x00000004,   ///< Boolean (true / false / 1 / 0)
        XType_Base_Blob         = 0x00000008,   ///< Binary large object
        XType_Base_String       = 0x00000010,   ///< String
        XType_Base_Action       = 0x00000020,   ///< Action (button)
        XType_Base_Rfu1         = 0x00000040,   ///< RFU
        XType_Base_Rfu2         = 0x00000080,   ///< RFU

        XType_Base_MinMax       = 0x00002000,   ///< The property accepts the strings 'min' and 'max' to set the best achievable extremities.
        XType_Base_ReadOnce     = 0x00001000,   ///< Property needs to be read at start-up only
        XType_Base_NoPersist    = 0x00000800,   ///< Property shouldn't be persisted (saved & restored)
        XType_Base_NAI          = 0x00000400,   ///< Property does not affect image intensity level ('Not Affecting Image')
        XType_Base_RW           = 0x00000300,   ///< Write and read back
        XType_Base_Writeable    = 0x00000200,   ///< Writeable properties have this set in their high byte
        XType_Base_Readable     = 0x00000100,   ///< Readable properties have this set in their high byte

        XType_Number            = 0x00000201,   ///< Write only number
        XType_Enum              = 0x00000202,   ///< Write only enumeration
        XType_Bool              = 0x00000204,   ///< Write only boolean
        XType_Blob              = 0x00000208,   ///< Write only binary large object
        XType_String            = 0x00000210,   ///< Write only string
        XType_Action            = 0x00000220,   ///< Action (button)

        XType_RO_Number         = 0x00000101,   ///< Read only number
        XType_RO_Enum           = 0x00000102,   ///< Read only enumeration
        XType_RO_Bool           = 0x00000104,   ///< Read only boolean
        XType_RO_Blob           = 0x00000108,   ///< Read only binary large object
        XType_RO_String         = 0x00000110,   ///< Read only string

        XType_RW_Number         = 0x00000301,   ///< R/W number
        XType_RW_Enum           = 0x00000302,   ///< R/W enumeration
        XType_RW_Bool           = 0x00000304,   ///< R/W boolean
        XType_RW_Blob           = 0x00000308,   ///< R/W binary large object
        XType_RW_String         = 0x00000310    ///< R/W string

    } XPropType;

    ///
    /// @brief Xeneth directory identifiers used by XCamera::GetPath
    ///
    typedef enum
    {
        XDir_FilterData         = 0x0000,   ///< Filter data (%APPDATA%/XenICs/Data/&lt;sessionnumber&gt;)
        XDir_ScriptRoot         = 0x0001,   ///< Script root (%APPDATA%/XenICs/Interface/&lt;PID-number&gt;)
        XDir_Calibrations       = 0x0002,   ///< Calibration folder (%ProgramFiles%/Xeneth/Calibrations)
        XDir_InstallDir         = 0x0003,   ///< Installation folder (%CommonProgramFiles%/XenICs/Runtime)
        XDir_Plugins            = 0x0004,   ///< Plugin folder (%CommonProgramFiles%/XenICs/Runtime/Plugins)
        XDir_CachePath          = 0x0005,   ///< Cache folder (%APPDATA%/XenICs/Cache)
        XDir_SdkResources       = 0x0006,   ///< SDK resource folder (%CommonProgramFiles%/XenICs/Runtime/Resources)
        XDir_Xeneth             = 0x0007    ///< Xeneth installation directory
    } XDirectories;


    ///
    ///  @brief Xeneth Enumeration Flags (XEF)
    ///
    ///  Used by #XCD_EnumerateDevices and XCamera::GetDeviceList to specify the protocols to be discovered.
    ///  Choose #XEF_EnableAll to enumerate all supported protocols. Two flags are made available for internal cache control:
    ///  #XEF_UseCached is passed when the user only wants to retrieve the devices already discovered by a previous
    ///  call to one of the discovery methods. #XEF_ReleaseCache allows the user to clear the internal cache list.
    ///  By default the cache list is cleared when a new discovery sequence is initiated or when the library is released.
    ///
    typedef enum
    {
        XEF_Network         = 0x00000001,   ///< Network
        XEF_Serial          = 0x00000002,   ///< Serial
        XEF_CameraLink      = 0x00000004,   ///< CameraLink
        XEF_GigEVision      = 0x00000008,   ///< GigEVision
        XEF_CoaXPress       = 0x00000010,   ///< CoaXPress
        XEF_USB             = 0x00000020,   ///< USB
        XEF_EnableAll       = 0x0000FFFF,   ///< Enable all protocols.
        XEF_UseCached       = 0x01000000,   ///< Use cached devices on enumeration.
        XEF_ReleaseCache    = 0x02000000,   ///< Release internally cached devices.
    } XEnumerationFlag;
    typedef unsigned int XEnumerationFlags; ///< @sa XEnumerationFlag

    ///
    /// @brief Xeneth device state (XDS) used in the XDeviceInformation structure to indicate the current state of the device.
    ///
    /// Devices which are marked as available are free to establish a connection to. Busy devices are currently
    /// in use by another user. Unreachable are devices which were detected but no route exists. In this case the
    /// user will be unable to establish a connection.
    ///
    typedef enum {
        XDS_Available = 0x0,    ///< The device is available to establish a connection.
        XDS_Busy = 0x1,         ///< The device is currently in use.
        XDS_Unreachable = 0x2   ///< The device was detected but is unreachable.
    } XDeviceStates;
    typedef unsigned int XDeviceState; ///< @sa XDeviceStates


/* Structures */
#pragma pack(push, 1)

    ///
    /// @brief Per frame software footer.
    ///
    /// Use XCamera::GetFrameFooterLength() to determine the exact size in bytes of both soft and hardware footers.
    /// For more detailed information about footers refer to the XFooters.h-header. To learn how to retrieve
    /// the per frame footer make sure to check XCamera::GetFrame(FrameType, unsigned long, void *, unsigned int) and the #XGF_FetchPFF-flag.
    ///
    typedef struct
    {
        unsigned short len;     ///< Structure length.
        unsigned short ver;     ///< Fixed to 0xAA00.

        long long      soc;     ///< Time of Start Capture (us since start of epoch).
        long long      tft;     ///< Time of reception (us since start of epoch).
        dword          tfc;     ///< Frame counter.
        dword          fltref;  ///< Reference for attaching messages/frame (described in #XFooters.h).
        dword          hfl;     ///< Hardware footer length.
    } XPFF;


    ///
    /// @brief   Device information structure
    ///
    /// This structure holds all details needed to set up a connection to a discovered device.
    /// Use this structure in combination with the XCD_EnumerateDevices()-function.
    /// In case a direct connection using a protocol specific URL was established it is also
    /// possible to catch the #XDeviceInformation-structure in the #XStatus-callback.
    ///
    typedef struct {
        int size;                   ///< Should be set to sizeof(XDeviceInformation)
        char name[64];              ///< String containing the device's model name
        char transport[64];         ///< Serial | CoaXPress | GigEVision | Network | CameraLink
        char url[256];              ///< URL identifying the camera internally. e.g. cam://0..n
        char address[64];           ///< The address where the device resides, the format is protocol specific. e.g. 192.168.2.2 | COM0..n | CL0..n::( NationalInstruments | Euresys | Imperx ... )
        unsigned int serial;        ///< Serial number reported by the camera. e.g. 0x12345678
        unsigned int pid;           ///< Product id reported by the camera. e.g. 0x0000F020
        XDeviceState state;         ///< Camera status to determine if the camera is in use at that time.
    } XDeviceInformation;

#pragma pack(pop)

#ifdef __cplusplus
    }
#endif

#ifdef __cplusplus

    //////////////////////////////////////////////////////////////////////////////////////
    ///
    /// @brief This class provides an interface to the Xenics cameras
    ///
    class XCamera
    {
        protected:
            XCamera();

        public:
            //////////////////////////////////////////////////////////////////////////////////////////
            //  Opening the camera
            //////////////////////////////////////////////////////////////////////////////////////////

            ///
            /// @brief Enumerates all available devices.
            ///
            /// A device list containing 'connectionURL'|'description'. Device names are separated by a pipe '|' symbol.
            ///
            /// @note It is advised to use the \ref c_device_discovery "Xeneth camera discovery functions (XCD_*)", such as XCD_EnumerateDevices to 
            /// perform device enumeration.
            /// @warning This function can be marked as deprecated in a future major release but is kept for now to maintain backwards compatibility.
            ///
            /// @param[out] pList       Zero terminated string which will receive the device list.
            /// @param[in]  iMaxLen     Allocated memory available for the device list.
            ///
            static void IMPEXP GetDeviceList(char * pList, int iMaxLen);
            
            
            ///
            /// @brief Enumerates all available devices, protocol selection is based on the value passed with the flags argument.
            ///
            /// A device list containing 'connectionURL'|'description'. Device names are separated by a pipe '|' symbol.
            ///
            /// @note It is advised to use the \ref c_device_discovery "Xeneth camera discovery functions (XCD_*)", such as XCD_EnumerateDevices to 
            /// perform device enumeration.
            /// @warning This function can be marked as deprecated in a future major release but is kept for now to maintain backwards compatibility.
            ///
            /// @param[out] pList       Zero terminated string which will receive the device list.
            /// @param[in]  iMaxLen     Allocated memory available for the device list.
            /// @param[in]  flags       Flag to configure the protocols to use during the discovery sequence or to retrieve
            ///                         the internal cached devices (See #XEnumerationFlag).
            ///
            static void IMPEXP GetDeviceList(char * pList, int iMaxLen, XEnumerationFlags flags);

            
            ///
            /// @brief Creates a camera object.
            ///
            /// @param[in]  pCameraName The camera connection URL as returned by GetDeviceList (!)\n
            ///                         Special cameras:\n
            ///                         \a "cam://select"  Start Xeneth (if installed) to select a camera\n
            ///                         \a "soft://0" Virtual camera (for application development without a camera)\n
            ///                         Options:\n
            ///                         \a "cam://x?fg=none" Start API in command & control mode (C&C). In this mode it is not possible to perform image acquisition.\n
            ///                         \a "cam://x?fg=XFrameGrabberNative" Given the choice between using a CameraLink&trade; grabber and the native protocol (ether/usb..) use the native one.
            ///
            /// @param[in]  pCallBack   Status callback function (See #XStatus)
            /// @param[in]  pUser       User parameter for said callback function
            ///
            /// @return A pointer to the created #XCamera-object, or a null pointer in case of an allocation error.
            ///
            static XCamera IMPEXP *Create(const char *pCameraName = "cam://0", XStatus pCallBack = 0, void *pUser = 0);

            
            ///
            /// @brief Call this destructor using the delete-operator to destroy the instance.
            ///
            /// The session to the camera will be closed when the destructor is called.
            ///
            virtual ~XCamera();

            
            ///
            /// @brief Checks if the session to the camera was properly initialised.
            ///
            /// @retval true    The session to the camera is successfully initialised
            /// @retval false   The session to the camera failed to initialise
            ///
            virtual bool IsInitialised() = 0;

            //////////////////////////////////////////////////////////////////////////////////////////
            // Frame dimensions
            //////////////////////////////////////////////////////////////////////////////////////////

            
            ///
            /// @brief The current width in number of pixels.
            ///
            /// @return Width in number of pixels.
            ///
            virtual dword GetWidth() = 0;

       
            ///
            /// @brief The current height in number of pixels.
            ///
            /// @return Height in number of pixels.
            ///
            virtual dword GetHeight() = 0;

            
            ///
            /// @brief The maximum width in number of pixels.
            ///
            /// @return Width in pixels.
            ///
            virtual dword GetMaxWidth() = 0;

            
            ///
            /// @brief The maximum height in number of pixels.
            ///
            /// @return Height in pixels.
            ///
            virtual dword GetMaxHeight() = 0;

            
            //////////////////////////////////////////////////////////////////////////////////////////
            // Capture control
            //////////////////////////////////////////////////////////////////////////////////////////

            ///
            /// @brief Check if the internal frame acquisition thread is currently acquiring frame data.
            ///
            /// @return True if the capture thread is active.
            ///
            virtual bool IsCapturing() = 0;

            
            ///
            /// @brief Enables the internal frame acquisition thread.
            ///
            /// @return This method will return #I_OK on success.
            ///         On failure one of the other possible values is returned. (see #ErrorCodes)
            ///
            /// @retval #I_OK   Success.
            ///
            virtual ErrCode StartCapture() = 0;

            
            ///
            /// @brief Disables the internal frame acquisition thread.
            ///
            /// @return This method will return #I_OK on success.
            ///         On failure one of the other possible values is returned. (see #ErrorCodes)
            ///
            /// @retval #I_OK   Success.
            ///
            virtual ErrCode StopCapture() = 0;

            //////////////////////////////////////////////////////////////////////////////////////////
            // Data filters
            //////////////////////////////////////////////////////////////////////////////////////////

            ///
            /// @brief Registers an image filter callback.
            ///
            /// @note The image filter callbacks are executed in the frame acquisition thread.
            ///
            /// @param[in]  flt     #XImageFilter function pointer.
            /// @param[in]  parm    User-data pointer. This argument will get passed as-is to the
            ///                     #XImageFilter-callback for each call.
            ///
            /// @return     A handle to the registered filter.
            ///
            virtual FilterID AddImageFilter(XImageFilter flt, void * parm) = 0;

            
            ///
            /// @brief Sends a message to an image filter.
            ///
            /// @param[in]  fid     Filter ID.
            /// @param[in]  msg     Message to the filter. (refer to #XFilterMessage)
            /// @param[in,out]
            ///             msgparm Parameters to pass to the image filter in question.
            ///
            /// @return     This method will return #I_OK on success.
            ///             On failure one of the other possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK   Success.
            ///
            virtual ErrCode MsgImageFilter(FilterID fid, XFilterMessage msg, void * msgparm) = 0;

            
            ///
            /// @brief Removes an image filter using the filter identifier.
            ///
            /// @param[in]  fid     Filter identifier.
            ///
            virtual void RemImageFilter(FilterID fid) = 0;

            
            ///
            /// @brief Set the filter's position in the filter stack.
            ///
            /// @param[in]  fid     Filter identifier.
            /// @param[in]  prio    Set the new position of the filter.
            ///                     When set to 0 the filter is positioned at the top of the stack.
            ///                     When given a negative number the filter is positioned in front
            ///                     of the filter with id prio.
            ///
            virtual void PriImageFilter(FilterID fid, int prio) = 0;

            
            ///
            /// @brief Checks if a filter has completed its run cycle.
            ///
            /// @param[in]  fid     Filter identifier.
            ///
            /// @return     bool
            /// @retval     true    The filter has completed its run cycle.
            /// @retval     false   The filter has not completed its run cycle.
            ///
            virtual bool IsFilterRunning(FilterID fid) = 0;

            //////////////////////////////////////////////////////////////////////////////////////////
            // Drawing
            //////////////////////////////////////////////////////////////////////////////////////////

            ///
            /// @brief Blits an image to a native window handle (HWND / GtkWidget ...).
            ///
            /// @param[in]  where   Handle to a window (HWND) or Device context handle (HDC).
            /// @param[in]  x       X coordinate of destination, upper-left corner.
            /// @param[in]  y       Y coordinate of destination upper-left corner.
            /// @param[in]  w       Width of destination rectangle.
            /// @param[in]  h       Height of destination rectangle.
            /// @param[in]  type    Specifies whether or not the where-argument points to a
            ///                     window or a DC (See #BlitType).
            ///
            virtual void Blit(void * where, int x, int y, int w, int h, BlitType type) = 0;

            
            ///
            /// @brief Sets the blitter's colour mode.
            ///
            /// @param[in]  mode    Enumerated type of colours.
            ///
            virtual void SetColourMode(ColourMode mode) = 0;

            
            ///
            /// @brief Gets the blitter's colour mode.
            ///
            /// @return     Enumerated type of colours.
            ///
            /// @retval     #ColourMode_8       Intensity only
            /// @retval     #ColourMode_16      Alias
            /// @retval     #ColourMode_Profile Uses a colour profile bitmap. See #LoadColourProfile()
            /// @retval     #ColourMode_Invert  Set this flag if an inversion of the colour profile is desired.
            ///                                 e.g.: #ColourMode_8 | #ColourMode_Invert
            ///
            virtual ColourMode GetColourMode() = 0;

            
            ///
            /// @brief Loads a colour profile bitmap from disk.
            ///
            /// To be used in conjunction with SetColourMode 's <b>Profile</b> mode.
            /// Colour profile bitmaps should be 1x256 pixels. Bigger is OK, but only (0,0->255) will be utilised.
            ///
            /// @param[in]  p_cFileName     The file to load the colour profile from.
            ///
            /// @return This method will return #I_OK on success.
            ///         On failure one of the other possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK   Success.
            ///
            virtual ErrCode LoadColourProfile(const char * p_cFileName) = 0;

            
            ///
            /// @brief Retrieves an index from the current palette
            ///
            /// @param[in]  ndx                 The index for the colour value in the palette.
            /// @param[out] red, green, blue    The colour components found at ndx in the colour profile.
            ///
            /// @return This method will return #I_OK on success.
            ///         On failure one of the other possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK   Success.
            ///
            virtual ErrCode GetPaletteNdx(unsigned int ndx, byte & red, byte & green, byte & blue) = 0;

            //////////////////////////////////////////////////////////////////////////////////////////
            // Data acquisition
            //////////////////////////////////////////////////////////////////////////////////////////

            ///
            /// @brief Gets a pointer to the internal frame buffer.
            ///
            /// The number of bits used in the data is rounded up to the next whole byte.
            /// For example, a camera that produces frame data at 14-bit resolution is stored
            /// in a 16-bit frame buffer. Refer to GetBitSize() and GetFrameType() to learn
            /// the native pixel size.
            ///
            /// @note This function may \b only be used inside an image filter running in the
            /// image acquisition thread.
            ///
            /// @return Pointer to the internal frame buffer.
            ///
            virtual void * GetFrame() = 0;

            
            ///
            /// @brief Returns the native frame type of this camera (See #FrameType).
            ///
            /// @return The native frame type of this camera
            ///
            virtual FrameType GetFrameType() = 0;

            
            ///
            /// @brief Returns the frame buffer size in bytes.
            ///
            /// @return Number of bytes.
            ///
            virtual dword GetFrameSize() = 0;

            
            ///
            /// @brief Retrieve the bits per pixel.
            ///
            /// @return Bits per pixel.
            ///
            virtual byte GetBitSize() = 0;

            
            ///
            /// @brief Retrieve the maximum pixel value.
            ///
            /// @return Maximum pixel value.
            ///
            virtual dword GetMaxValue() = 0;

            
            ///
            /// @brief Copies a captured frame from the internal buffers to the user supplied buffer.
            ///
            /// @warning Do not use this function from inside an #XImageFilter-callback. If this is the
            /// case then make sure to use #GetFrame() instead.
            ///
            /// <b>Asynchronous mode: </b>
            /// When the #XGF_Blocking-flag is cleared the method works asynchronously by default.
            /// In this mode the function will return immediately with its return value set to #I_OK if a
            /// frame was successfully copied to the supplied buffer. If no frame was available the method
            /// will return #E_NO_FRAME indicating that no frame was copied to the buffer.
            /// Any other value on return indicates that an error occurred.
            ///
            /// <b>Blocking mode: </b>
            /// When the #XGF_Blocking-flag is set the method will only return until a frame was successfully
            /// copied to the buffer or when the time-out period has elapsed. In this mode, when a frame
            /// was successfully copied to the supplied buffer the method will return #I_OK.
            /// If the time-out period elapsed, and no frame was copied to the buffer, the method will
            /// return #E_NO_FRAME. To configure the frame time-out period refer to the
            /// _API_GETFRAME_TIMEOUT-property (@ref props_application).
            ///
            /// <b>Per frame footer: </b>
            /// Internally a software footer (#XPFF) is appended to each frame. Next to that, some cameras support
            /// a hardware footer which is located immediately after this software footer. To retrieve these
            /// footers a buffer has to be supplied that is large enough to accommodate the frame data and
            /// both the software and hardware footers. Use GetFrameFooterLength() to retrieve the length in
            /// bytes for both footers. The footer is copied to the supplied buffer when the #XGF_FetchPFF-flag
            /// is set in the ulFlags-argument.
            /// @note The size-argument should still only contain the size of the frame minus the size
            /// of the footer or the method will return the #E_MISMATCHED error code.
            ///
            /// <b>Internal 8-bit conversion: </b>
            /// The #XGF_NoConversion-flag prevents internal conversion of the captured frame to an internal
            /// 8-bit buffer. By setting this flag it is possible to reduce the computation time per frame.
            /// @note   When #XGF_NoConversion-flag is set both SaveData() and the Blit() methods are disabled.
            ///
            /// @param[in]  type                Supported conversion types: #FT_32_BPP_RGBA / #FT_32_BPP_RGB (24bpp colour triplets)
            ///                                 or #FT_NATIVE (no conversion)
            /// @param[in]  ulFlags             This argument can have 0 or any of the #XGetFrameFlags combined.
            /// @param[out] buffer              User supplied buffer to accommodate the captured frame.
            /// @param[in]  size                Size in bytes of the user supplied buffer.
            ///
            /// @return                         This method will return #I_OK on success.
            ///                                 On failure one of the other possible values is returned. (see #ErrorCodes)
            /// @retval     #I_OK               Success.
            /// @retval     #E_NO_FRAME         No frames are available in the internal buffers to copy to the supplied buffer.
            ///                                 This is a special return value since this is not really an error but
            ///                                 more of an indication to the user if a frame was copied to the destined buffer.
            ///                                 In blocking mode this return value will indicate a time-out period has elapsed.
            /// @retval     #E_NOINIT           Camera was not successfully initialised.
            /// @retval     #E_NO_CONVERSION    The requested conversion is not supported.
            /// @retval     #E_MISMATCHED       The supplied buffer size does not match with the size of the requested frame type.
            ///
            virtual ErrCode GetFrame(FrameType type, unsigned long ulFlags, void * buffer, unsigned int size) = 0;

            
            ///
            /// @brief Gets the number of frames acquired since StartCapture.
            ///
            /// @return The number of frames acquired since the start of capture.
            ///
            virtual dword GetFrameCount() = 0;

            
            ///
            /// @brief Gets the current frame rate.
            ///
            /// @return The current rate of the frames being acquired.
            ///
            virtual double GetFrameRate() = 0;

            
            ///
            /// @brief Saves 8-bit image data to a bitmap
            ///
            /// Saves the 16-bit image data when saving Portable Network Graphics (PNG) if FullColour is set, or when using XSD_Force16
            /// For other formats, which do not support 16-bit image channels, the most significant bits are stored in the red component the others in the green component.
            ///
            /// @param[in]  ulFlags      see #XSaveDataFlags
            /// @param[in]  p_cFileName  Filename where the data is to be stored.
            ///
            /// @return     This method will return #I_OK on success.
            ///             On failure on of the possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #E_SAVE_ERROR   If this function fails on I/O.
            ///
            virtual ErrCode SaveData(const char *p_cFileName, unsigned long ulFlags = 0) = 0;

            
            //////////////////////////////////////////////////////////////////////////////////////////
            // Configuration management
            //////////////////////////////////////////////////////////////////////////////////////////

            ///
            /// @brief Loads camera settings from a configuration file.
            ///
            /// @param[in]  p_cFileName Filename to an .XCF file.
            /// @param[in]  ulFlags     *New in v2.1* (see #XLoadSettingsFlags)
            ///
            /// @return     This method will return #I_OK on success.
            ///             On failure on of the possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK       Success.
            ///
            virtual ErrCode LoadSettings(const char *p_cFileName, unsigned long ulFlags = 0) = 0;

            
            ///
            /// @brief Saves camera settings to a configuration file.
            ///
            /// @param[in]  p_cFileName Filename to an .XCF file.
            /// @param[in]  ulFlags     *New in v2.1* (see #XSaveSettingsFlags)
            ///
            /// @return     This method will return #I_OK on success.
            ///             On failure on of the possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK       Success.
            ///
            virtual ErrCode SaveSettings(const char *p_cFileName, unsigned long ulFlags = 0) = 0;

            
            ///
            /// @brief Loads sensor calibration data, and optionally starts the correction filter too.
            ///
            /// @param[in]  ulFlags      (see #XLoadCalibrationFlags)
            /// @param[in]  p_cFileName  Filename to the .XCA file you wish to load.
            ///
            /// @return     This method will return #I_OK on success.
            ///             On failure on of the possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK       Success.
            ///
            virtual ErrCode LoadCalibration(const char *p_cFileName, unsigned long ulFlags = 0) = 0;

            
            ///
            /// @brief Retrieve the location of directories used by the API.
            ///
            /// @param[in]  iPath       #XDirectories identifier.
            /// @param[out] pPath       Buffer used as the destination to copy the path.
            /// @param[in]  iMaxLen     Maximum number of allocated bytes for pPath.
            ///
            /// @return     This method will return #I_OK on success.
            ///             On failure on of the possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK       Success.
            ///
            virtual ErrCode GetPath(int iPath, char *pPath, int iMaxLen) = 0;

            
            //////////////////////////////////////////////////////////////////////////////////////////
            // Property management
            //////////////////////////////////////////////////////////////////////////////////////////

            ///
            /// @brief Retrieves the number of properties available on the currently connected device.
            ///
            /// @return The number of properties available on the currently connected device.
            ///
            virtual int GetPropertyCount() = 0;

            
            ///
            /// @brief Retrieves the name of a property by its index
            ///
            /// @param[in]  iIndex       Legal values are 0 to XCamera::GetPropertyCount() - 1.
            /// @param[out] pPropName    Destination string pointer that will receive the name.
            /// @param[in]  iMaxLen      The number of bytes allocated for pPropName.
            ///
            /// @return     This method will return #I_OK on success.
            ///             On failure one of the other possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK   Success.
            ///
            virtual ErrCode GetPropertyName(int iIndex, char *pPropName, int iMaxLen) = 0;


            ///
            /// @brief Retrieves the boundaries of a property
            ///
            /// For an enumerated type this is a list of possible values, for numeric types it returns "lowvalue>highvalue".
            ///
            /// @param[in]  pPrp        The property name.
            /// @param[out] pRange      Destination pointer that will receive the range.
            /// @param[in]  iMaxLen     Reserved space in pRange.
            ///
            /// @return     This method will return #I_OK on success.
            ///             On failure one of the other possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK   Success.
            ///
            virtual ErrCode GetPropertyRange(const char *pPrp, char   *pRange   , int     iMaxLen   ) = 0;
            virtual ErrCode GetPropertyRangeL(const char *pPrp, long   *pLow     , long   *pHigh     ) = 0;    ///< Convenience function to extra higher & lower mark of numeric properties. (see #GetPropertyRange)
            virtual ErrCode GetPropertyRangeF(const char *pPrp, double *pLow     , double *pHigh     ) = 0;    ///< Convenience function to extra higher & lower mark of numeric properties. (see #GetPropertyRange)


            ///
            /// @brief Retrieves the type of a property
            ///
            /// @param[in]  pPrp        The property name (for example: "IntegrationTime", or it's categorized alternative "Camera/General/Integration time")
            /// @param[out] pPropType   The base property type plus its attributes (readable and/or writeable, .. see #XPropType)
            ///
            /// @return     This method will return #I_OK on success.
            ///             On failure one of the other possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK   Success.
            ///
            virtual ErrCode GetPropertyType(const char *pPrp, XPropType *pPropType) = 0;


            ///
            /// @brief Retrieves where this property is located in the property hierarchy.
            /// 
            /// This method is also used to retrieve the property's user level and display name.
            ///
            /// @param[in]  pPrp         The property name.
            /// @param[out] pCategory    A serialized tree representation of the categories separated by slashes.
            /// @param[in]  iMaxLen      Space reserved at pCategory.
            ///
            /// @return     This method will return #I_OK on success.
            ///             On failure one of the other possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK   Success.
            ///
            virtual ErrCode GetPropertyCategory(const char *pPrp, char *pCategory  , int iMaxLen) = 0;


            ///
            /// @brief Retrieve units supported by this property, default unit comes first
            ///
            /// @param[in]  pPrp        The property name.
            /// @param[out] pUnit       Destination for a comma separated list of supported units.
            /// @param[in]  iMaxLen     Maximum length of destination string.
            ///
            /// @return     This method will return #I_OK on success.
            ///             On failure one of the other possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK   Success.
            ///
            virtual ErrCode GetPropertyUnit(const char *pPrp, char *pUnit      , int iMaxLen ) = 0;


            ///
            /// @brief Sets the value of a named property
            ///
            /// @param[in]  pPrp     The property name (for example: "IntegrationTime", or it's categorized alternative "Camera/General/Integration time")
            /// @param[in]  pValue   The value to set the property to
            /// @param[in]  pUnit    RFU
            ///
            /// @return     This method will return #I_OK on success.
            ///             On failure one of the other possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK   Success.
            ///
            virtual ErrCode SetPropertyValue(const char *pPrp, const char *pValue       , const char *pUnit = ((const char*)0)) = 0;
            virtual ErrCode SetPropertyValueL(const char *pPrp, long lValue              , const char *pUnit = ((const char*)0)) = 0;   ///< Available for easy access. (see #SetPropertyValue)
            virtual ErrCode SetPropertyValueF(const char *pPrp, double dValue            , const char *pUnit = ((const char*)0)) = 0;   ///< Available for easy access. (see #SetPropertyValue)
            virtual ErrCode SetPropertyBlob(const char *pPrp, const char *pValue       , unsigned int len) = 0;                         ///< Available for setting large binary objects.

            
            ///
            /// @brief Fetches the value of a named property
            ///
            /// For write-only properties this retrieves the last set, or default, value.
            ///
            /// @param[in]  pPrp     The property name
            /// @param[out] pValue   A pointer that will receive the value.
            /// @param[in]  iMaxLen  The number of bytes reserved in the destination pointer pValue
            ///
            /// @return     This method will return #I_OK on success.
            ///             On failure one of the other possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK   Success.
            ///
            virtual ErrCode GetPropertyValue(const char * pPrp, char * pValue, int iMaxLen) = 0;
            virtual ErrCode GetPropertyValueL(const char * pPrp, long * pValue) = 0;                    ///< Available for easy access. (see #GetPropertyValue)
            virtual ErrCode GetPropertyValueF(const char * pPrp, double * pValue) = 0;                  ///< Available for easy access. (see #GetPropertyValue)
            virtual ErrCode GetPropertyBlob(const char * pPrp, char * pValue, unsigned int len) = 0;    ///< Available for getting large binary objects, use #XCamera::GetPropertyValueL to determine how many bytes to allocate, or overestimate the len field.

            
            ///
            /// @brief Control the transaction of new property values by queueing property changes.
            ///
            /// Calling this function with a value of true causes property changes to be cached until the next call of this function with a value of false.
            /// This is a nestable operation using a reference count.
            ///
            /// @return     This method will return #I_OK on success.
            ///             On failure one of the other possible values is returned. (see #ErrorCodes)
            ///
            /// @retval     #I_OK   Success.
            ///
            virtual ErrCode QueueProperties(bool tf) = 0;


            ///
            /// @brief Returns the length of the XPFF structure plus any hardware frame footer.
            ///
            /// @warning    Never use sizeof(#XPFF) to determine the length of the frame footer. The hardware footer can have a
            ///             variable length and the actual size can only be determined with confidence by retrieving the length
            ///             from the software footer (XPFF::hfl)
            ///
            /// @return     #XPFF's size in bytes.
            ///
            virtual dword GetFrameFooterLength() = 0;

            
            ///
            /// @brief Retrieve the list of entries for an enumeration property.
            ///
            /// When the call was successful pRange will return a formatted string containing for each entry both
            /// the entry's short name and display name. This short name is used as the value for both the
            /// #GetPropertyValueE and #SetPropertyValueE methods. The display name is a user friendly string which
            /// can be displayed in the user interface of the application.
            ///
            /// The format for the returned string is: &quot;<b>short-name</b>=<b>display-name</b>,<b>short-name</b>=<b>display-name</b> ...&quot;\n
            /// This string exists of multiple key-value pairs delimited by a <b>comma-character</b> ",".
            /// Each entity of the key-value pair is delimited by an '<b>equals sign</b>'-character "=".
            /// Because the comma and equals sign are reserved characters it is necessary to encode these characters in the display name.
            /// For this the display name is encoded using a similar technique to entity encoding.
            /// The format is &<b>nnn</b>; where <b>nnn</b> is the code for a printable character in decimal form.
            ///
            /// @param[in]  pPrp            The property name.
            /// @param[out] pRange          Initialised character buffer to retrieve the enum-property range.
            /// @param[in]  iMaxLen         Reserved space in pRange.
            ///
            /// @return                         This method will return #I_OK on success.
            ///                                 On failure on of the possible values is returned. (see #ErrorCodes)
            /// @retval     #I_OK               Success.
            /// @retval     #E_NOT_SUPPORTED    The camera does not support this property.
            /// @retval     #E_MISMATCHED       Buffer size mismatch.
            /// @retval     #E_BUG              Generic failure. e.g. a NULL pointer argument was passed to the method.
            ///
            /// @since      Xeneth 2.6
            ///
            virtual ErrCode GetPropertyRangeE(const char * pPrp, char * pRange, int iMaxLen) = 0;


            ///
            /// @brief      Sets the value of an enumeration property using the name of the enumeration entry.
            ///
            /// @param[in]  pPrp            The property name.
            /// @param[in]  pValue          A character buffer which contains the new named value of the enum-property.
            ///
            /// @returns    ErrCode             This method will return #I_OK on success.
            ///                                 On failure on of the possible values is returned. (see #ErrorCodes)
            /// @retval     #I_OK               Success.
            /// @retval     #E_NOT_SUPPORTED    The camera does not support this property.
            /// @retval     #E_NOT_FOUND        The named entry was not found.
            /// @retval     #E_BUG              Generic failure. e.g. a NULL pointer argument was passed to the method.
            ///
            /// @since      Xeneth 2.6
            ///
            virtual ErrCode SetPropertyValueE(const char * pPrp, const char *pValue) = 0;


            ///
            /// @brief      Retrieves the named entry corresponding to the current value of an enumeration property.
            ///
            /// @param[in]  pPrp            The property name.
            /// @param[out] pValue          An initialised character buffer to retrieve the current .
            ///                             short name of the enumeration-property.
            /// @param[in]  iMaxLen         The number of bytes reserved for pValue.
            ///
            /// @returns                        This method will return #I_OK on success.
            ///                                 On failure on of the possible values is returned. (see #ErrorCodes)
            /// @retval     #I_OK               Success.
            /// @retval     #E_NOT_SUPPORTED    The camera does not support this property.
            /// @retval     #E_MISMATCHED       Buffer size mismatch.
            /// @retval     #E_BUG              Generic failure. e.g. a NULL pointer argument was passed to the method.
            ///
            /// @since      Xeneth 2.6
            ///
            virtual ErrCode GetPropertyValueE(const char * pPrp, char * pValue, int iMaxLen) = 0;

    };
#endif /* __cplusplus */

#ifdef __cplusplus
    extern "C"
    {
#endif
        ///////////////////////////////////////////////////////////////////////////////////
        //	C Functions Definition
        ///////////////////////////////////////////////////////////////////////////////////
        /// @defgroup	c_api	XCamera C Functions
        ///	@{
        ///
        /// The following functions serve as a C interface wrapper to the XCamera-object.
        ///
        ///////////////////////////////////////////////////////////////////////////////////
        XCHANDLE    IMPEXPC XC_OpenCamera                   (const char * pCameraName = "cam://default", XStatus pCallBack = 0, void * pUser = 0);  ///< @sa XCamera::Create
        void        IMPEXPC XC_CloseCamera                  (XCHANDLE hnd);                                                                         ///< @sa XCamera::~XCamera
        void        IMPEXPC XC_GetDeviceList                (char * pList, int iMaxLen);                                                            ///< @sa XCamera::GetDeviceList
        void        IMPEXPC XC_GetDeviceListEx              (char * pList, int iMaxLen, XEnumerationFlags flags);                                   ///< @sa XCamera::GetDeviceList

        voidp       IMPEXPC XC_HandleToCamera               (XCHANDLE hHandle);                                                                     ///< Converts a C style handle to an XCamera instance pointer
        XCHANDLE    IMPEXPC XC_CameraToHandle               (voidp cam);                                                                            ///< Converts an XCamera instance pointer to a C style handle (For use in image filter callbacks)
        int         IMPEXPC XC_ErrorToString                (ErrCode e, char * dst, int len);                                                       ///< Converts an error code to a string, returns how many bytes were copied.

        boole       IMPEXPC XC_IsInitialised                (XCHANDLE h);                                                                           ///< @sa XCamera::IsInitialised
        dword       IMPEXPC XC_GetWidth                     (XCHANDLE h);                                                                           ///< @sa XCamera::GetWidth
        dword       IMPEXPC XC_GetHeight                    (XCHANDLE h);                                                                           ///< @sa XCamera::GetHeight
        dword       IMPEXPC XC_GetMaxWidth                  (XCHANDLE h);                                                                           ///< @sa XCamera::GetMaxWidth
        dword       IMPEXPC XC_GetMaxHeight                 (XCHANDLE h);                                                                           ///< @sa XCamera::GetMaxHeight
        boole       IMPEXPC XC_IsCapturing                  (XCHANDLE h);                                                                           ///< @sa XCamera::IsCapturing
        ErrCode     IMPEXPC XC_StartCapture                 (XCHANDLE h);                                                                           ///< @sa XCamera::StartCapture
        ErrCode     IMPEXPC XC_StopCapture                  (XCHANDLE h);                                                                           ///< @sa XCamera::StopCapture
        FilterID    IMPEXPC XC_AddImageFilter               (XCHANDLE h, XImageFilter flt, void * parm);                                            ///< @sa XCamera::AddImageFilter
        ErrCode     IMPEXPC XC_MsgImageFilter               (XCHANDLE h, FilterID fid, XFilterMessage msg, void * msgparm);                         ///< @sa XCamera::MsgImageFilter
        void        IMPEXPC XC_RemImageFilter               (XCHANDLE h, FilterID fid);                                                             ///< @sa XCamera::RemImageFilter
        void        IMPEXPC XC_PriImageFilter               (XCHANDLE h, FilterID fid, int prio);                                                   ///< @sa XCamera::PriImageFilter
        boole       IMPEXPC XC_IsFilterRunning              (XCHANDLE h, FilterID fid);                                                             ///< @sa XCamera::IsFilterRunning
        void        IMPEXPC XC_Blit                         (XCHANDLE h, void * w, int x, int y, int width, int height, BlitType type);             ///< @sa XCamera::Blit
        void        IMPEXPC XC_SetColourMode                (XCHANDLE h, ColourMode mode);                                                          ///< @sa XCamera::SetColourMode
        ColourMode  IMPEXPC XC_GetColourMode                (XCHANDLE h);                                                                           ///< @sa XCamera::GetColourMode
        ErrCode     IMPEXPC XC_LoadColourProfile            (XCHANDLE h, const char * p_cFileName);                                                 ///< @sa XCamera::LoadColourProfile
        voidp       IMPEXPC XC_GetFilterFrame               (XCHANDLE h);                                                                           ///< @sa XCamera::GetFrame()
        FrameType   IMPEXPC XC_GetFrameType                 (XCHANDLE h);                                                                           ///< @sa XCamera::GetFrameType
        dword       IMPEXPC XC_GetFrameSize                 (XCHANDLE h);                                                                           ///< @sa XCamera::GetFrameSize
        byte        IMPEXPC XC_GetBitSize                   (XCHANDLE h);                                                                           ///< @sa XCamera::GetBitSize
        dword       IMPEXPC XC_GetMaxValue                  (XCHANDLE h);                                                                           ///< @sa XCamera::GetMaxValue
        ErrCode     IMPEXPC XC_GetFrame                     (XCHANDLE h, FrameType type, unsigned long ulFlags, void * buffer, unsigned int size);  ///< @sa XCamera::GetFrame(FrameType, unsigned long, void *, unsigned int)
        dword       IMPEXPC XC_GetFrameCount                (XCHANDLE h);                                                                           ///< @sa XCamera::GetFrameCount
        double      IMPEXPC XC_GetFrameRate                 (XCHANDLE h);                                                                           ///< @sa XCamera::GetFrameRate
        ErrCode     IMPEXPC XC_SaveData                     (XCHANDLE h, const char * p_cFileName, unsigned long ulFlags);                          ///< @sa XCamera::SaveData
        ErrCode     IMPEXPC XC_LoadSettings                 (XCHANDLE h, const char * p_cFileName);                                                 ///< @sa XCamera::LoadSettings
        ErrCode     IMPEXPC XC_LoadCalibration              (XCHANDLE h, const char * p_cFileName, unsigned long ulFlags);                          ///< @sa XCamera::LoadCalibration
        ErrCode     IMPEXPC XC_SaveSettings                 (XCHANDLE h, const char * p_cFileName);                                                 ///< @sa XCamera::SaveSettings
        ErrCode     IMPEXPC XC_GetPath                      (XCHANDLE h, int iPath, char * pPath, int iMaxLen);                                     ///< @sa XCamera::GetPath
        int         IMPEXPC XC_GetPropertyCount             (XCHANDLE h);                                                                           ///< @sa XCamera::GetPropertyCount
        ErrCode     IMPEXPC XC_GetPropertyName              (XCHANDLE h, int iIndex, char * pPropName, int iMaxLen);                                ///< @sa XCamera::GetPropertyName
        ErrCode     IMPEXPC XC_GetPropertyRange             (XCHANDLE h, const char * pPrp, char * pRange, int iMaxLen);                            ///< @sa XCamera::GetPropertyRange
        ErrCode     IMPEXPC XC_GetPropertyRangeL            (XCHANDLE h, const char * pPrp, long * pLow, long * pHigh);                             ///< @sa XCamera::GetPropertyRangeL
        ErrCode     IMPEXPC XC_GetPropertyRangeF            (XCHANDLE h, const char * pPrp, double * pLow, double * pHigh);                         ///< @sa XCamera::GetPropertyRangeF
        ErrCode     IMPEXPC XC_GetPropertyRangeE            (XCHANDLE h, const char * pPrp, char * pRange, int iMaxLen);                            ///< @sa XCamera::GetPropertyRangeE
        ErrCode     IMPEXPC XC_GetPropertyType              (XCHANDLE h, const char * pPrp, XPropType * pPropType);                                 ///< @sa XCamera::GetPropertyType
        ErrCode     IMPEXPC XC_GetPropertyCategory          (XCHANDLE h, const char * pPrp, char * pCategory, int iMaxLen);                         ///< @sa XCamera::GetPropertyCategory
        ErrCode     IMPEXPC XC_GetPropertyUnit              (XCHANDLE h, const char * pPrp, char * pUnit, int iMaxLen);                             ///< @sa XCamera::GetPropertyUnit
        ErrCode     IMPEXPC XC_SetPropertyValue             (XCHANDLE h, const char * pPrp, const char * pValue, const char * pUnit);               ///< @sa XCamera::SetPropertyValue
        ErrCode     IMPEXPC XC_SetPropertyValueL            (XCHANDLE h, const char * pPrp, long lValue, const char * pUnit);                       ///< @sa XCamera::SetPropertyValueL
        ErrCode     IMPEXPC XC_SetPropertyValueF            (XCHANDLE h, const char * pPrp, double dValue, const char * pUnit);                     ///< @sa XCamera::SetPropertyValueF
        ErrCode     IMPEXPC XC_SetPropertyValueE            (XCHANDLE h, const char * pPrp, const char * pValue);                                   ///< @sa XCamera::SetPropertyValueE
        ErrCode     IMPEXPC XC_SetPropertyBlob              (XCHANDLE h, const char * pPrp, const char * pValue, unsigned int len);                 ///< @sa XCamera::SetPropertyBlob
        ErrCode     IMPEXPC XC_GetPropertyValue             (XCHANDLE h, const char * pPrp, char * pValue, int iMaxLen);                            ///< @sa XCamera::GetPropertyValue
        ErrCode     IMPEXPC XC_GetPropertyValueL            (XCHANDLE h, const char * pPrp, long * pValue);                                         ///< @sa XCamera::GetPropertyValueL
        ErrCode     IMPEXPC XC_GetPropertyValueF            (XCHANDLE h, const char * pPrp, double * pValue);                                       ///< @sa XCamera::GetPropertyValueF
        ErrCode     IMPEXPC XC_GetPropertyValueE            (XCHANDLE h, const char * pPrp, char * pValue, int iMaxLen);                            ///< @sa XCamera::GetPropertyValueE
        ErrCode     IMPEXPC XC_GetPropertyBlob              (XCHANDLE h, const char * pPrp, char * pValue, unsigned int len);                       ///< @sa XCamera::GetPropertyBlob
        ErrCode     IMPEXPC XC_QueueProperties              (XCHANDLE h, bool tf);                                                                  ///< @sa XCamera::QueueProperties
        dword       IMPEXPC XC_GetFrameFooterLength         (XCHANDLE h);                                                                           ///< @sa XCamera::GetFrameFooterLength

        ///////////////////////////////////////////////////////////////////////////////////
        /// @}
        /// @defgroup   c_device_discovery  Device discovery functions
        /// @note It is not advised to use one of the obsolete #XCamera::GetDeviceList-methods.
        /// Although for backwards compatibility these will not be removed in the near future, still the new functions 
        /// are introduced to make it faster and easier for the developer to enumerate devices and avoid having to parse 
        /// a pipe-delimited string to learn the discovered devices.
        ///
        /// Make sure to check out the \ref devicediscovery "device discovery" sample to learn how enumerations are handled.
        /// @{
        ///////////////////////////////////////////////////////////////////////////////////

        ///
        /// @brief Enumerate devices using the protocols specified by the flags-argument.
        ///
        /// When the deviceInformation-argument is a null pointer enumeration will occur but only the deviceCount-argument
        /// will be updated with the number of discovered devices. In this use case any device found on the separate protocols
        /// will be cached internally. By knowing the device count the user is able to allocate enough memory to accommodate
        /// all the discovered devices. After allocating enough memory, the cached device can be retrieved faster by making sure
        /// that on the the next call the #XEF_UseCached-flag is set. Copying of the cached devices into the new allocated array
        /// will happen almost instantaneously compared to performing another enumeration.
        /// @note This function will not perform any enumeration if no protocol flags were set.
        ///
        /// @param[out]     deviceInformation   A pointer to an array of #XDeviceInformation structures holding the discovered devices when a call returned successfully.
        /// @param[in,out]  deviceCount         When the deviceInformation-argument is a null pointer this argument's direction is OUT
        ///                                     and holds the number of the discovered devices. If a valid pointer was passed to deviceInformation
        ///                                     this argument's direction is IN and holds the number of allocated #XDeviceInformation-entries.
        /// @param[in]      flags               #XEnumerationFlags are used to indicate which protocols a discovery is performed on.
        ///                                     These flags can be combined to enumerate multiple protocols at once.
        ///
        /// @return         This function returns #I_OK on success. On failure this function returns one of the possible error codes.
        ///                 Check #ErrorCodes for more information about the returned error code.
        ///
        /// @retval         #I_OK               Success.
        /// @retval         #E_BUG              The deviceCount-argument cannot be a null pointer.
        ///
        ErrCode IMPEXPC XCD_EnumerateDevices(XDeviceInformation * const deviceInformation, unsigned int * const deviceCount, XEnumerationFlags flags);

        
        ///
        /// @brief Retrieve the discovery properties' count.
        ///
        /// This function is mainly used to get the count of the number of properties present in the device discovery property system.
        ///
        /// Using the return value an index can be calculated to retrieve the property name using #XCD_GetPropertyName.
        ///
        /// @return     The number of properties available in the device discovery property system.
        ///
        dword IMPEXPC XCD_GetPropertyCount(void);

        
        ///
        /// @brief Retrieve the property name based on its index. 
        ///
        /// This name is then further used as an identifier to all other property functions such as #XCD_GetPropertyCategory, 
        /// #XCD_GetPropertyRange, #XCD_GetPropertyType, #XCD_GetPropertyValue, #XCD_SetPropertyValue ..
        ///
        /// @param[in]  idx                 The index of the property within the property system.
        /// @param[out] propertyName        A pointer to a user allocated character array to which the name is copied on success.
        /// @param[in]  maxLen              The maximum size allocated to propertyName.
        ///
        /// @return     This function returns #I_OK on success. On failure this function returns one of the possible error codes.
        ///             Check #ErrorCodes for more information about the returned error code.
        ///
        /// @retval     #I_OK               Success.
        /// @retval     #E_BUG              A null pointer was passed to one of the pointer arguments.
        /// @retval     #E_NOT_FOUND        The property identified by its name was not found.
        /// @retval     #E_MISMATCHED       The supplied range buffer's size is too small to accommodate the property's name.
        ///
        ErrCode IMPEXPC XCD_GetPropertyName(dword idx, char * propertyName, dword maxLen);

        
        ///
        /// @brief Retrieve the property category identified by its name.
        ///
        /// @param[in]  propertyName        A pointer to a character array holding the property name to look-up.
        /// @param[out] propertyCategory    A pointer to a user allocated character array to which the category name is copied on success.
        /// @param[in]  maxLen              The maximum size allocated to propertyCategory.
        ///
        /// @return     This function returns #I_OK on success. On failure this function returns one of the possible error codes.
        ///             Check #ErrorCodes for more information about the returned error code.
        ///
        /// @retval     #I_OK           Success.
        /// @retval     #E_BUG          A null pointer was passed to one of the pointer arguments.
        /// @retval     #E_NOT_FOUND    The property identified by its name was not found.
        /// @retval     #E_MISMATCHED   The supplied range buffer's size is too small to accommodate the property's category.
        ///
        ErrCode IMPEXPC XCD_GetPropertyCategory(const char * propertyName, char * propertyCategory, int maxLen);

        
        ///
        /// @brief Retrieve the type of the property identified by its name.
        ///
        /// @param[in]  propertyName        A pointer to a character array holding the property name to look-up.
        /// @param[out] propertyType        A pointer to a user allocated XPropType used to return the type of the requested property.
        ///
        /// @return     This function returns #I_OK on success. On failure this function returns one of the possible error codes.
        ///             Check #ErrorCodes for more information about the returned error code.
        ///
        /// @retval     #I_OK           Success.
        /// @retval     #E_BUG          A null pointer was passed to one of the pointer arguments.
        /// @retval     #E_NOT_FOUND    The property identified by its name was not found.
        ///
        ErrCode IMPEXPC XCD_GetPropertyType(const char * propertyName, XPropType * propertyType);

        
        ///
        /// @brief Retrieve the property value in string format identified by its name.
        ///
        /// If the property's underlying type is not of a string type conversions will be performed.
        ///
        /// @param[in]  propertyName        A pointer to a character array holding the property name to look-up.
        /// @param[out] value               A pointer to a user allocated character array to which the property value is copied on success.
        /// @param[in]  maxLen              The maximum size allocated to the value pointer.
        ///
        /// @return     This function returns #I_OK on success. On failure this function returns one of the possible error codes.
        ///             Check #ErrorCodes for more information about the returned error code.
        ///
        /// @retval     #I_OK           Success.
        /// @retval     #E_BUG          A null pointer was passed to one of the pointer arguments.
        /// @retval     #E_NOT_FOUND    The property identified by its name was not found.
        ///
        ErrCode IMPEXPC XCD_GetPropertyValue(const char * propertyName, char * value, dword maxLen);

        
        ///
        /// @brief Retrieve the property integer value identified by its name.
        ///
        /// @note that not all properties, such as string typed properties, support conversion to integer types and as a result the
        /// function will return with an error.
        ///
        /// @param[in]  propertyName        A pointer to a character array holding the property name to look-up.
        /// @param[out] value               A pointer to a user allocated integer to which the property value is copied on success.
        ///
        /// @return     This function returns #I_OK on success. On failure this function returns one of the possible error codes.
        ///             Check #ErrorCodes for more information about the returned error code.
        ///
        /// @retval     #I_OK           Success.
        /// @retval     #E_BUG          A null pointer was passed to one of the pointer arguments.
        /// @retval     #E_NOT_FOUND    The property identified by its name was not found.
        ///
        ErrCode IMPEXPC XCD_GetPropertyValueL(const char * propertyName, int * value);

        
        ///
        /// @brief Set the property value in string format identified by its name.
        ///
        /// @param[in]  propertyName        A pointer to a character array holding the property name to look-up.
        /// @param[in]  value               A pointer to a user allocated character array holding the new value of the property on success.
        /// @param[in]  len                 The maximum size allocated to value.
        ///
        /// @return     This function returns #I_OK on success. On failure this function returns one of the possible error codes.
        ///             Check #ErrorCodes for more information about the returned error code.
        ///
        /// @retval     #I_OK           Success.
        /// @retval     #E_BUG          A null pointer was passed to one of the pointer arguments.
        /// @retval     #E_NOT_FOUND    The property identified by its name was not found.
        ///
        ErrCode IMPEXPC XCD_SetPropertyValue(const char * propertyName, const char * value, dword len);

        
        ///
        /// @brief Set the property integer value identified by its name.
        ///
        /// @param[in]  propertyName        A pointer to a character array holding the property name to look-up.
        /// @param[in]  value               The new value of the property on success.
        ///
        /// @return     This function returns #I_OK on success. On failure this function returns one of the possible error codes.
        ///             Check #ErrorCodes for more information about the returned error code.
        ///
        /// @retval     #I_OK           Success.
        /// @retval     #E_BUG          A null pointer was passed to one of the pointer arguments.
        /// @retval     #E_NOT_FOUND    The property identified by its name was not found.
        ///
        ErrCode IMPEXPC XCD_SetPropertyValueL(const char * propertyName, int value);

        
        ///
        /// @brief Retrieve the property range identified by its name.
        ///
        /// The property ranges are primarily used when its underlying type is an integer or enumeration.
        /// For these types it is then possible to define and validate the minimum and maximum boundaries.
        /// Enumeration types will return strings in the form of "Option1,Option2,Option3".
        /// Integers will return strings in the form of "0>1" where zero is the lower boundary and 1 is the upper boundary.
        ///
        /// @param[in]  propertyName        A pointer to a character array holding the property name to look-up.
        /// @param[out] range               A pointer to a user allocated character array to which the range is copied on success.
        /// @param[in]  maxLen              The maximum size allocated to range.
        ///
        /// @return     This function returns #I_OK on success. On failure this function returns one of the possible error codes.
        ///             Check #ErrorCodes for more information about the returned error code.
        ///
        /// @retval     #I_OK           Success.
        /// @retval     #E_BUG          A null pointer was passed to one of the pointer arguments.
        /// @retval     #E_NOT_FOUND    The property identified by its name was not found.
        /// @retval     #E_MISMATCHED   The supplied range buffer's size is too small to accommodate the property's range.
        ///
        ErrCode IMPEXPC XCD_GetPropertyRange(const char * propertyName, char * const range, int maxLen);

        ///////////////////////////////////////////////////////////////////////////////////
        /// @}
        ///////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
    }
#endif

#endif
