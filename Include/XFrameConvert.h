/** \file XFrameConvert.h */
#ifndef _XENICS_XFRAMECONVERTER_H_
#   define _XENICS_XFRAMECONVERTER_H_

/**/////////////////////////////////////////////////////////////////////////////
//// XenicsAPI - Interface definition
////////////////////////////////////////////////////////////////////////////////

#include <XCamera.h>

#ifdef __cplusplus
    extern "C" {
#endif


typedef int XFCHANDLE;  ///< handle to XFrameConvert

///
/// @brief The list of possible frame conversions.
///
/// The frame conversion type is passed as an argument during construction of the
/// #XFrameConvert-class. (see XFrameConvert::Create() and XFC_OpenFrameConvert()).
///
typedef enum
{
    XFC_Correction = 0  ///< A raw image is corrected applying the data from a Xenics correction (.XCA) file.
} XFrameConversionType;

#ifdef __cplusplus
    }
#endif



#ifdef __cplusplus
    ///
    /// @brief This class provides an interface to correct raw frames off-line.
    ///
    class XFrameConvert
    {
    protected:
        XFrameConvert() {};

    public:
        ///
        /// @brief Creates a file converter.
        ///        
        /// @warning only a single XFrameConvert instance can exist simultaneously.
        /// Before you can create an new instance with other parameters it is necessary
        /// to delete the current instance.
        ///
        /// @param[in]  width           The width of the frames to be converted
        /// @param[in]  height          The height of the frames to be converted
        /// @param[in]  pixelSize       The bit depth of the pixels composing the frames
        /// @param[in]  conversionType  see #XFrameConversionType
        ///
        /// @return Returns a pointer to a constructed #XFrameConvert-instance.
        ///
        static XFrameConvert IMPEXP * Create(int width, int height, int pixelSize, XFrameConversionType conversionType);

        
        ///
        /// @brief Call this destructor using the delete-operator to destroy the instance.
        ///
        virtual ~XFrameConvert() {};

        
        ///
        /// @brief Converts the source frame
        ///
        /// @param[in]  SrcFrame        Pointer to the source frame
        /// @param[in]  SrcFrameSize    The size of the source frame in bytes: ( width * height * pixelSize ) + footerSize
        /// @param[out] DstFrame        Pointer to a buffer. When this pointer is non zero, the source frame will be unaffected while the converted frame is copied into the this buffer.
        /// @param[in]  DstFrameSize    Size of the buffer
        ///
        /// @return This method will return #I_OK on success.
        ///         On failure one of the other possible values is returned. (see #ErrorCodes)
        ///
        /// @retval #I_OK   Success.
        ///
        virtual ErrCode Convert(void * SrcFrame, int SrcFrameSize, void * DstFrame = (void *)0, int DstFrameSize = 0 ) = 0;

        
        ///
        /// @brief Sets the value of a named property
        ///
        /// @param[in]  pPrp            The property name (for example: "IntegrationTime").
        /// @param[in]  pValue          The value to set the property to.
        /// @param[in]  pUnit           Reserved for future use (RFU).
        ///
        /// @return This method will return #I_OK on success.
        ///         On failure one of the other possible values is returned. (see #ErrorCodes)
        ///
        /// @retval #I_OK   Success.
        ///
        virtual ErrCode SetPropertyValue(const char * pPrp, const char * pValue, const char * pUnit = ((const char *)0))=0;
        virtual ErrCode SetPropertyValueL(const char * pPrp, long lValue, const char * pUnit = ((const char *)0))=0;    ///< Available for easy access. (see #SetPropertyValue)
        virtual ErrCode SetPropertyValueF(const char * pPrp, double dValue, const char * pUnit = ((const char *)0))=0;  ///< Available for easy access. (sea #SetPropertyValue)

        ///
        /// @brief Fetches the value of a named property
        ///
        /// For non camera readable properties this retrieves the value last set, or the default.
        ///
        /// @param[in]  pPrp            The property name (for example: "IntegrationTime")
        /// @param[out] pValue          A pointer that will receive the value.
        /// @param[in]  iMaxLen         The number of bytes reserved in the destination pointer pValue
        ///
        /// @return This method will return #I_OK on success.
        ///         On failure one of the other possible values is returned. (see #ErrorCodes)
        ///
        /// @retval #I_OK   Success.
        ///
        virtual ErrCode GetPropertyValue(const char * pPrp, char * pValue, int iMaxLen) = 0;
        virtual ErrCode GetPropertyValueL(const char * pPrp, long * pValue) = 0;    ///< Available for easy access. (see #GetPropertyValue)
        virtual ErrCode GetPropertyValueF(const char * pPrp, double * pValue) = 0;  ///< Available for easy access. (see #GetPropertyValue)

        ///
        /// @brief Loads the file that contains the correction data.
        ///
        /// @param[in]  p_cFileName     The full path to the calibration data. For instance "C:\\Temp\\ConvertFile\\AutoExposure_2086.xca"
        ///
        /// @return This method will return #I_OK on success.
        ///         On failure one of the other possible values is returned. (see #ErrorCodes)
        ///
        /// @retval #I_OK   Success.
        ///
        virtual ErrCode     LoadCalibration     (const char *p_cFileName )= 0;
    };


#endif //__cplusplus

#ifdef __cplusplus
    extern "C" {
#endif

    XFCHANDLE IMPEXPC XFC_OpenFrameConvert(int width, int height, int pixelSize, XFrameConversionType type);            ///< @sa XFrameConvert::Create
    void IMPEXPC XFC_CloseFrameConvert(XFCHANDLE h);                                                                    ///< @sa XFrameConvert::~XFrameConvertor
    ErrCode IMPEXPC XFC_Convert(XFCHANDLE h, void * pSrcFrame, int iSrcFrameSize, void * pDstFrame, int iDstFrameSize); ///< @sa XFrameConvert::Convert
    
    ErrCode IMPEXPC XFC_SetPropertyValue(XFCHANDLE h,const char * pPrp, const char * pValue, const char * pUnit);       ///< @sa XFrameConvert::SetPropertyValue
    ErrCode IMPEXPC XFC_SetPropertyValueL(XFCHANDLE h,const char * pPrp, long lValue, const char * pUnit);              ///< @sa XFrameConvert::SetPropertyValueL
    ErrCode IMPEXPC XFC_SetPropertyValueF(XFCHANDLE h,const char * pPrp, double dValue, const char * pUnit);            ///< @sa XFrameConvert::SetPropertyValueF

    ErrCode IMPEXPC XFC_GetPropertyValue(XFCHANDLE h,const char * pPrp, char * pValue, int iMaxLen);                    ///< @sa XFrameConvert::GetPropertyValue
    ErrCode IMPEXPC XFC_GetPropertyValueL(XFCHANDLE h,const char * pPrp, long * pValue);                                ///< @sa XFrameConvert::GetPropertyValueL
    ErrCode IMPEXPC XFC_GetPropertyValueF(XFCHANDLE h,const char * pPrp, double * pValue);                              ///< @sa XFrameConvert::GetPropertyValueF

    ErrCode IMPEXPC XFC_LoadCalibration(XCHANDLE h, const char * p_cFileName);                                          ///< @sa XFrameConvert::LoadCalibration

#ifdef __cplusplus
    }
#endif

#endif //_XENICS_XFRAMECONVERTER_H_
