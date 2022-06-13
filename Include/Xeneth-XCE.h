#ifndef _XENICS_XCALIBRATIONEDITOR_H_
#define _XENICS_XCALIBRATIONEDITOR_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file       Xeneth-XCE.h
/// @author     Xenics
///
/// @brief		Xenics Calibration Editor - Interface Definitions.
///
///             This file contains the API definition used by the Xeneth Calibration Editor.
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "XCamera.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int XCEHANDLE;  ///< handle to XCalibrationEditor

///
/// @brief Type of image
///
typedef enum {
    IM_Dark     = 0,    ///< 'Dark' image
    IM_Light    = 1,    ///< 'Light' (grey) image
    IM_TrueNUC  = 2     ///< 'Dark' image with integration time, for TrueNUC data
} ImageMode;


///
/// @brief Class of pixel
///
typedef enum {
    BPM_Bad       = -1, ///< 'Bad' pixel
    BPM_Undefined = 0,  ///< 'Undefined' leaves the pixel state up to the detection algorithms or previously loaded values from the calibration pack
    BPM_Good      = 1   ///< 'Good' pixel
} BadPixelMarking;


///
/// @brief Type of bad pixel
///
typedef enum {
    BP_FPN        = 1 << 0, ///< Pixel marked due to excessive Fixed Pattern Noise (dark current)
    BP_NoiseDark  = 1 << 1, ///< Pixel marked due to excessive Temporal Noise (dark image)
    BP_NoiseLight = 1 << 2, ///< Pixel marked due to excessive Temporal Noise (light image)
    BP_PRNU       = 1 << 3, ///< Pixel marked due to deviating Photon Response Non-Uniformity (gain)
    BP_Linearity  = 1 << 4, ///< Pixel marked due to poor TrueNUC fit
    BP_User       = 1 << 7, ///< Pixel marked bad by user
    BP_UGood      = 1 << 15 ///< Pixel marked good by user
} BadPixelTypes;
typedef int BadPixelType;   ///< @sa BadPixelTypes


///
/// @brief Parts of the calibration procedure
///
typedef enum {
    CM_FPN     = 1,             ///< Fixed Pattern Noise (dark current) calculation
    CM_PRNU    = 2,             ///< Photon Response Non-Uniformity (gain) calculation
    CM_TrueNUC = 4,             ///< TrueNUC fit
    CM_BP      = 8              ///< Bad pixel detection
} CalibrationModes;

typedef int CalibrationMode;    ///< @sa CalibrationModes

///
/// Status callback prototype
///
/// @param status - status message
///
/// @return     This method will return #I_OK on success.
///             On failure one of the other possible values is returned. (see #ErrorCodes)
///
/// @retval     #I_OK   Success.
///
typedef ErrCode (CALLCVCB *XCEStatus)(const char *status);

#ifdef __cplusplus
}
///
/// @brief This class provides an interface to read and edit a Xenics calibration pack.
///
class XCalibrationEditor
{
protected:
    XCalibrationEditor() {};

public:
    ///
    /// @brief Creates a calibration editor.
    ///
    /// @param[in]  width           The width of the frames to be used for calibration
    /// @param[in]  height          The height of the frames to be used for calibration
    /// @param[in]  callback        Pointer to a status callback function to receive additional logging info
    ///
    /// @return Returns a pointer to an #XCalibrationEditor-instance.
    ///
    static XCalibrationEditor IMPEXP *Create(size_t width, size_t height, XCEStatus callback = 0);


    /// @todo TODO
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    static ErrCode IMPEXP GetVersion(char *version, int iMaxLen);


    ///
    /// @brief Call this destructor using the delete-operator to destroy the instance.
    ///
    virtual ~XCalibrationEditor();


    ///
    /// @brief Gets the number of properties in the .XCF settings-file of the calibration.
    ///
    /// @return the number of properties in the .XCF settings-file of the calibration.
    ///
    virtual int IMPEXP GetPropertyCount() = 0;


    ///
    /// @brief Retrieves the name of an .XCF property by its index.
    ///
    /// @param[in]  iIndex          Legal values are 0 to GetPropertyCount() - 1
    /// @param[out] pPropName       Destination string pointer that will receive the name
    /// @param[in]  iMaxLen         Amount of space reserved at pPropName
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP GetPropertyName(int iIndex, char *pPropName, int iMaxLen) = 0;


    ///
    /// @brief Retrieves the type of a property of an .XCF.
    ///
    /// @param[in]  pPrp            The property name (for example: "IntegrationTime")
    /// @param[out] pPropType       The base property type plus it's attributes (readable / writable) (see #XPropType)
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP GetPropertyType(const char *pPrp, XPropType *pPropType) = 0;


    ///
    /// @brief Sets the value of a named property of an .XCF.
    ///
    /// @param[in]  pPrp            The property name (for example: "IntegrationTime")
    /// @param[in]  pValue          The value to set the property to
    /// @param[in]  pUnit           RFU
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP SetPropertyValue( const char *pPrp, const char *pValue, const char *pUnit = ((const char*)0)) = 0;
    virtual ErrCode IMPEXP SetPropertyValueL(const char *pPrp, long lValue,        const char *pUnit = ((const char*)0)) = 0;  ///< Available for easy access. (see #SetPropertyValue)
    virtual ErrCode IMPEXP SetPropertyValueF(const char *pPrp, double dValue,      const char *pUnit = ((const char*)0)) = 0;  ///< Available for easy access. (see #SetPropertyValue)


    ///
    /// @brief Fetches the value of a named property of an .XCF.
    ///
    /// For non camera readable properties this retreives the value last set, or the default.
    ///
    /// @param[in]  pPrp            The property name (for example: "IntegrationTime")
    /// @param[out] pValue          A pointer that will receive the value.
    /// @param[in]  iMaxLen         The number of bytes reserved in the destination pointer pValue
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP GetPropertyValue( const char *pPrp, char *pValue, int iMaxLen) = 0;
    virtual ErrCode IMPEXP GetPropertyValueL(const char *pPrp, long *pValue) = 0;              ///< Available for easy access. (see #GetPropertyValue)
    virtual ErrCode IMPEXP GetPropertyValueF(const char *pPrp, double *pValue) = 0;            ///< Available for easy access. (see #GetPropertyValue)

    ///
    /// @brief Manually mark a pixel as 'bad', 'undetermined' or 'good'.
    ///
    /// When a pixel is set 'undetermined', it may still be marked by the algorithm if it is
    /// outside the supplied bad pixel limits. (see #XCalibrationEditor::BadPixelLimits).
    ///
    /// @param[in]  x               X-coordinate of the pixel
    /// @param[in]  y               Y-coordinate of the pixel
    /// @param[in]  mark            Indication to mark as 'bad', 'undetermined' or 'good' (see #BadPixelMarking)
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP MarkBadPixel(size_t x, size_t y, BadPixelMarking mark) = 0;


    ///
    /// @brief Add an image to the calibration data.
    ///
    /// @param[in]  mode            Part of the calibration data the image is meant for. (see #ImageMode)
    /// @param[in]  image           Pointer to the image data
    /// @param[in]  integrationTime Integration time used to take the image (only has an effect for TrueNUC calibrations)
    /// @param[in]  footer          Indicates that the provided image contains an extra line of footer-data
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP AddImageData(ImageMode mode, unsigned short *image, size_t integrationTime = 0, bool footer = false) = 0;


    ///
    /// @brief Retrieve a processed image from the calibration data.
    ///
    /// An image can only be retrieved if images have previously been added by AddImageData() for the requested mode. The image will only
    /// contain valid data if image data for the requested mode was added to the calibration.
    ///
    /// @param[in]  mode            Part of the calibration data that is requested. (see #ImageMode)
    /// @param[out] image           Pointer to write the requested image data to, must have a buffer allocated of suitable size
    /// @param[in]  integrationTime Integration time used to take the image (only has an effect for TrueNUC calibrations)
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP GetAverageImage(ImageMode mode, unsigned short *image, size_t integrationTime = 0) = 0;

    ///
    /// @brief Retrieve a processed image from the calibration data.
    /// @copydetails XCalibrationEditor::GetAverageImage()
    ///
    virtual ErrCode IMPEXP GetNoiseImage(ImageMode mode, unsigned short *image, size_t integrationTime = 0) = 0;


    ///
    /// @brief Retrieve a map with selected bad pixels.
    ///
    /// The user must indicate which bad pixel types must be included in the map. The map will only contain data if image data
    /// was added to the calibration, and after the calibration was calculated with mode CM_BP. (see #CalibrationModes)
    ///
    /// @param[out] image            Pointer to write the requested map to, must have a buffer allocated of suitable size
    /// @param[in]  type             Type(s) of bad pixel(s) that should be included in the map (see #BadPixelTypes)
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP GetBadPixelMap(unsigned short *image, BadPixelType type) = 0;


    ///
    /// @brief Set the parameters of the bad pixel detection.
    ///
    /// The detection ranges can be passed as a percentage of the standard deviation on the image (default), or by absolute ADU count.
    ///
    /// @param[in]  fpn             Range for bad pixels based on 'fixed pattern noise' (dark current)
    /// @param[in]  noiseDark       Range for bad pixels based on noise on the 'dark' image
    /// @param[in]  noiseLight      Range for bad pixels based on noise on the 'light' image
    /// @param[in]  prnu            Range for bad pixels based on 'photon response non-uniformity' (gain)
    /// @param[in]  absolute        Indicates that the provided ranges are in percent (false) or ADU (true)
    /// @param[in]  linearity       Range for bad pixels based on linearity of response to integration time (only has an effect for TrueNUC calibrations)
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP BadPixelLimits(size_t fpn, size_t noiseDark, size_t noiseLight, size_t prnu, bool absolute = false, size_t linearity = 350) = 0;


    ///
    /// @brief Set the 'KeepOffset' flag, to indicate whether the dark current needs to be normalised or subtracted.
    ///
    /// @param[in]  enabled         Indicates normalised (true) or subtracted (false) dark current
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP KeepOffset( bool enabled) = 0;


    ///
    /// @brief Set an additional pixel gain value after non-uniformity correction.
    ///
    /// @param[in]  enabled         Indicates the use of an extra gain (true), or not (false)
    /// @param[in]  gain            Gain value
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP PostNUCGain(bool enabled, float gain = -1) = 0;


    ///
    /// @brief Set line camera status and number of banks that need calibration.
    ///
    /// @param[in]  enabled         Indicates if it's a line camera (true) or a 2D camera (false)
    /// @param[in]  linecount       The number of 'sample & hold' banks on the camera
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP LineCamera(bool enabled, size_t linecount) = 0;


    ///
    /// @brief Set the parameters to create a TrueNUC calibration.
    ///
    /// Packs to be uploaded to the camera should have 'degree' = 1, software packs are 'degree' = 5.
    ///
    /// @param[in]  degree          Degree of the fitted curve, maximum 5
    /// @param[in]  tintmin         Minimum integration time supported by the calibration
    /// @param[in]  tintmax         Maximum integration time supported by the calibration
    /// @param[in]  tintdefault     Default (startup) integration time for the calibration
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP TrueNUC(size_t degree, size_t tintmin, size_t tintmax, size_t tintdefault) = 0;


    ///
    /// @brief Set the parameters for additional sensor temperature compensation.
    ///
    /// This feature is only supported by a subset of TrueNUC-capable cameras.
    ///
    /// @param[in]  enabled                  Indicates the use of sensor temperature compensation
    /// @param[in]  sensortempcoefficient    Coefficient to calculate the compensation map
    /// @param[in]  coolingtemp              Cooling temp that was set to create the calibration
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP TrueNUCSensorTemp(bool enabled, float sensortempcoefficient, float coolingtemp) = 0;


    ///
    /// @brief Calculate the calibration data after the measurement data was added.
    ///
    /// If no image data was added for a requested calibration mode, no new calibration data for that mode will be calculated or saved.
    ///
    /// @param[in]  mode            The part of the calibration data that needs (re)calculating (see #CalibrationModes)
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP CalculateCalibration(CalibrationMode mode = -1) = 0;


    ///
    /// @brief Load an existing calibration pack.
    ///
    /// @param[in]  path            Location of the existing calibration pack.
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP LoadCalibration(const char *path) = 0;


    ///
    /// @brief Save the calibration data to a pack.
    ///
    /// @param[in]  path            Location of the resulting calibration pack
    ///
    /// @return     This method will return #I_OK on success.
    ///             On failure one of the other possible values is returned. (see #ErrorCodes)
    ///
    /// @retval     #I_OK   Success.
    ///
    virtual ErrCode IMPEXP SaveCalibration(const char *path) = 0;
};

extern "C" {
#endif

    ///////////////////////////////////////////////////////////////////////////////////
	//	C Functions Definition
    ///////////////////////////////////////////////////////////////////////////////////
    /// @defgroup   c_xce   XCE C Functions
    ///
    ///	@brief		These are the wrapper methods for use in C applications.
    ///
    ///				These methods have the same functionality as the XCalibrationEditor methods with the only difference that an XCEHANDLE is used to keep a reference to the object.
    ///             Such XCEHANDLE is initialized when the XCE_OpenCalibrationEditor method is successfully called.
    ///
    /// @addtogroup c_xce
    /// @{

    XCEHANDLE IMPEXPC XCE_OpenCalibrationEditor(size_t width, size_t height, XCEStatus callback);                                                   ///< @sa XCalibrationEditor::Create
    void IMPEXPC XCE_CloseCalibrationEditor(XCEHANDLE h);                                                                                           ///< @sa XCalibrationEditor::~XCalibrationEditor

    ErrCode IMPEXPC XCE_GetVersion(char * version, int iMaxLen);                                                                                    ///< @sa XCalibrationEditor::GetVersion

    //  XCF property handling
    int IMPEXPC XCE_GetPropertyCount(XCEHANDLE h);                                                                                                  ///< @sa XCalibrationEditor::GetPropertyCount
    ErrCode IMPEXPC XCE_GetPropertyName(XCEHANDLE h, int iIndex, char * pPropName, int iMaxLen);                                                    ///< @sa XCalibrationEditor::GetPropertyName
    ErrCode IMPEXPC XCE_GetPropertyType(XCEHANDLE h, const char * pPrp, XPropType * pPropType);                                                     ///< @sa XCalibrationEditor::GetPropertyType

    ErrCode IMPEXPC XCE_SetPropertyValue(XCEHANDLE h, const char * pPrp, const char *pValue, const char * pUnit);                                   ///< @sa XCalibrationEditor::SetPropertyValue
    ErrCode IMPEXPC XCE_SetPropertyValueL(XCEHANDLE h, const char * pPrp, long lValue, const char * pUnit);                                         ///< @sa XCalibrationEditor::SetPropertyValueL
    ErrCode IMPEXPC XCE_SetPropertyValueF(XCEHANDLE h, const char * pPrp, double dValue, const char * pUnit);                                       ///< @sa XCalibrationEditor::SetPropertyValueF

    ErrCode IMPEXPC XCE_GetPropertyValue(XCEHANDLE h, const char * pPrp, char * pValue, int iMaxLen);                                               ///< @sa XCalibrationEditor::GetPropertyValue
    ErrCode IMPEXPC XCE_GetPropertyValueL(XCEHANDLE h, const char * pPrp, long * pValue);                                                           ///< @sa XCalibrationEditor::GetPropertyValueL
    ErrCode IMPEXPC XCE_GetPropertyValueF(XCEHANDLE h, const char * pPrp, double * pValue);                                                         ///< @sa XCalibrationEditor::GetPropertyValueF

    // Calibration handling
    ErrCode IMPEXPC XCE_MarkBadPixel(XCEHANDLE, size_t x, size_t y, BadPixelMarking mark);                                                          ///< @sa XCalibrationEditor::MarkBadPixel

    ErrCode IMPEXPC XCE_AddImageData(XCEHANDLE h, ImageMode mode, unsigned short * image, size_t integrationTime, bool footer);                     ///< @sa XCalibrationEditor::AddImageData

    ErrCode IMPEXPC XCE_GetAverageImage(XCEHANDLE h, ImageMode mode, unsigned short * image, size_t integrationTime);                               ///< @sa XCalibrationEditor::GetAverageImage
    ErrCode IMPEXPC XCE_GetNoiseImage(XCEHANDLE h, ImageMode mode, unsigned short * image, size_t integrationTime);                                 ///< @sa XCalibrationEditor::GetNoiseImage
    ErrCode IMPEXPC XCE_GetBadPixelMap(XCEHANDLE h, unsigned short * image, BadPixelType type);                                                     ///< @sa XCalibrationEditor::GetBadPixelMap

    ErrCode IMPEXPC XCE_BadPixelLimits(XCEHANDLE h, size_t fpn, size_t noiseDark, size_t noiseLight, size_t prnu, bool absolute, size_t linearity); ///< @sa XCalibrationEditor::BadPixelLimits

    ErrCode IMPEXPC XCE_KeepOffset(XCEHANDLE h, bool enabled);                                                                                      ///< @sa XCalibrationEditor::KeepOffset
    ErrCode IMPEXPC XCE_PostNUCGain(XCEHANDLE h, bool enabled, float gain);                                                                         ///< @sa XCalibrationEditor::PostNUCGain
    ErrCode IMPEXPC XCE_LineCamera(XCEHANDLE h, bool enabled, size_t linecount);                                                                    ///< @sa XCalibrationEditor::LineCamera

    ErrCode IMPEXPC XCE_TrueNUC(XCEHANDLE h, size_t degree, size_t tintmin, size_t tintmax, size_t tintdefault);                                    ///< @sa XCalibrationEditor::TrueNUC
    ErrCode IMPEXPC XCE_TrueNUCSensorTemp(XCEHANDLE h, bool enabled, float sensortempcoefficient, float coolingtemp);                               ///< @sa XCalibrationEditor::TrueNUCSensorTemp

    ErrCode IMPEXPC XCE_CalculateCalibration(XCEHANDLE h, CalibrationMode mode);                                                                    ///< @sa XCalibrationEditor::CalculateCalibration

    ErrCode IMPEXPC XCE_LoadCalibration(XCEHANDLE h, const char * path);                                                                            ///< @sa XCalibrationEditor::LoadCalibration
    ErrCode IMPEXPC XCE_SaveCalibration(XCEHANDLE h, const char * path);                                                                            ///< @sa XCalibrationEditor::SaveCalibration

    /// @}

#ifdef __cplusplus
}
#endif

#endif //_XENICS_XCALIBRATIONEDITOR_H_
