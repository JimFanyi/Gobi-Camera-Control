///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file       Xeneth-XVI.h
///
/// @brief		XVI SDK Interface Definitions.
///
///             This file contains the interface definitions used in the XVI SDK.
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _XENICS_XVI_SDK_
#	define _XENICS_XVI_SDK_

#ifdef _WIN32
#   ifndef CALLCV
#       define CALLCV       __stdcall                       ///< Compatibility with VB6
#       define CALLCVCB     __cdecl
#   else
#       pragma message ("Warning: building using custom calling convention.")
#   endif
#   ifdef _USRDLL
#       define IMPEXP   __declspec(dllexport)               /**< Attributes for C++ exports */
#       define IMPEXPC  __declspec(dllexport) CALLCV        /**< Attributes for C   exports */
#   else
#       define IMPEXP   __declspec(dllimport)               /**< Attributes for C++ exports */
#       define IMPEXPC  __declspec(dllimport) CALLCV        /**< Attributes for C   exports */
#   endif
#else
#   define IMPEXP
#   define IMPEXPC
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	typedef unsigned int    dword;                   /**< 4 bytes (32 bits)                        */
    typedef unsigned short	word;                    /**< 2 bytes (16 bits)                        */
    typedef unsigned char	byte;                    /**< 1 byte  ( 8 bits)                        */
    typedef unsigned char	boole;                   /**< 1 byte  ( 8 bits)                        */
    typedef void *			voidp;                   /**< void pointer type                        */

	typedef enum
	{
		I_OK				= 0,					///< No errors occurred.
		E_NOT_FOUND			= 20000,				///< File, property or stream entry not found.
		E_FAIL				= 20001,				///< Failed. (see XviReader method documentation for possible reasons)
		E_INVALID_FORMAT	= 20002,				///< The given XVI is not valid.
		E_NOT_SUPPORTED		= 20003,				///< This method is not supported for this XVI.
		E_WRONG_BUFFERSIZE	= 20004,				///< The given buffer size is invalid.
		E_END_OF_FILE		= 20005,				///< End of file occurred.
		E_OUT_OF_RANGE		= 20006					///< The given index is out of range.
	} XviError;

	typedef enum
    {
        // Sensor pixel types
        FT_NATIVE       = 0,                        ///< The native frame type of this camera (can be FT_8..,FT_16..,FT32.. check GetFrameType())
        FT_8_BPP_GRAY   = 1,                        ///< 8bpp
        FT_16_BPP_GRAY  = 2,                        ///< 16bpp (default for most of our cameras)
        FT_32_BPP_GRAY  = 3,                        ///< 32bpp
        // Conversion types (FT_16_BPP_GRAY -> ...)
        FT_32_BPP_RGBA  = 4,                        ///< 32bpp colour RGBA [B, G, R, A] Available for output conversion in XC_GetFrame
        FT_32_BPP_RGB   = 5,                        ///< 32bpp colour RGB [B, G, R] Available for output conversion in XC_GetFrame
        FT_32_BPP_BGRA  = 6,                        ///< 32bpp colour BGRA [R, G, B, A]
        FT_32_BPP_BGR   = 7                         ///< 32bpp colour BGR [R, G, B]
    } FrameType;

	/** Property types */
    typedef enum
    {
        XType_None              = 0x00000000    , ///< No (invalid) type

        XType_Base_Enum         = 0x00000002    , ///< An enumerated type (a choice)
        XType_Base_Number       = 0x00000001    , ///< A number (floating)
        XType_Base_Bool         = 0x00000004    , ///< Boolean (true/false/1/0)
        XType_Base_Blob         = 0x00000008    , ///< Binary large object
        XType_Base_String       = 0x00000010    , ///< String
        XType_Base_Action       = 0x00000020    , ///< Action (button)
        XType_Base_Rfu1         = 0x00000040    , ///< Reserved for future use (RFU)
        XType_Base_Rfu2         = 0x00000080    , ///< Reserved for future use (RFU)
        XType_Base_Mask         = 0x000000ff    , ///< Type mask

        XType_Base_MinMax       = 0x00002000    , ///< The property accepts the strings 'min' and 'max' to set the best achievable extremities.
        XType_Base_ReadOnce     = 0x00001000    , ///< Property needs to be read at start-up only
        XType_Base_NoPersist    = 0x00000800    , ///< Property shouldn't be persisted (saved & restored)
        XType_Base_NAI          = 0x00000400    , ///< Property does not affect image intensity level ('Not Affecting Image')
        XType_Base_RW           = 0x00000300    , ///< Write and read back
        XType_Base_Writeable    = 0x00000200    , ///< Writable properties have this set in their high byte
        XType_Base_Readable     = 0x00000100    , ///< Readable properties have this set in their high byte

        XType_Number            = 0x00000201    , ///< Write only number
        XType_Enum              = 0x00000202    , ///< Write only enum
        XType_Bool              = 0x00000204    , ///< Write only boolean
        XType_Blob              = 0x00000208    , ///< Write only binary large object
        XType_String            = 0x00000210    , ///< Write only string
        XType_Action            = 0x00000220    , ///< Action (button)

        XType_RO_Number         = 0x00000101    , ///< Read only number
        XType_RO_Enum           = 0x00000102    , ///< Read only enum
        XType_RO_Bool           = 0x00000104    , ///< Read only boolean
        XType_RO_Blob           = 0x00000108    , ///< Read only binary large object
        XType_RO_String         = 0x00000110    , ///< Read only string

        XType_RW_Number         = 0x00000301    , ///< R/W number
        XType_RW_Enum           = 0x00000302    , ///< R/W enum
        XType_RW_Bool           = 0x00000304    , ///< R/W boolean
        XType_RW_Blob           = 0x00000308    , ///< R/W binary large object
        XType_RW_String         = 0x00000310      ///< R/W string
    } XPropType;

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

///
/// @brief This class provides an interface for reading XVI files.
///
/// This is the main interface to open and read the Xenics XVI file format in C++.
/// Refer to \ref c_xvi for the C wrappers functions.
///
class XviReader
{
public:
	///
	/// @brief		Create a new XviReader object.
	///
	///				Static method that returns a pointer to an XviReader object.
	///
	/// @retval		XviReader*			A pointer to a valid initialized XviReader object.
	///	@retval		NULL				Object creation failed.
	///
	static XviReader	IMPEXP * Create();

    ///////////////////////////////////////////////////////////////////////////////////
	//	Basic operations
    
	///
	/// @brief		Destructor.
	///
	///				When the destructor is called all allocated resources will be destroyed.
	///
	virtual					   ~XviReader();

    
	///
	///	@brief		Open an XVI File.
	///
	///				Will open the file and check if it's in a valid format.
	///
	///	@param[in]  filename			A '\0' terminated string.
	///
	/// @return This method will return #I_OK on success.
    ///         On failure one of the other possible values is returned. (see #XviError)
    ///
	/// @retval		#I_OK				File was opened and verified successfully.
	/// @retval		#E_NOT_FOUND		The given filename does not exist.
	/// @retval		#E_FAIL				Could not open the file.
	/// @retval		#E_INVALID_FORMAT	The given file is not a valid XVI file format.
	///
	virtual XviError	IMPEXP Open(const char* filename)=0;

    
	///
	///	@brief		Close the current opened XVI file.
	///
	///				Will close the currently opened file.
	///
	/// @return This method will return #I_OK on success.
    ///         On failure one of the other possible values is returned. (see #XviError)
    ///
	/// @retval		#I_OK				File was closed successfully.
	/// @retval		#E_FAIL				Could not close the file.
	///
	virtual XviError	IMPEXP Close()=0;

    
    ///////////////////////////////////////////////////////////////////////////////////
	//	Properties
    
	///
	///	@brief		Check if a file is currently opened.
	/// Will check if a file is currently opened by the reader and return.
	///
	///	@return		boole
    ///
	/// @retval		TRUE				When a file is currently opened.
	/// @retval		FALSE				When no file is opened.
	///
	virtual boole		IMPEXP IsOpened()=0;

	///
	///	@brief		Gets the major version.
	///	Will return the major version number of the currently opened file.
	///
	///	@return		unsigned long
	/// @retval		0					No file is opened at the moment.
	///
	virtual unsigned	IMPEXP long GetMajorversion()=0;


	///
	///	@brief		Gets the minor version.
	/// Will return the minor version number of the currently opened file.
	///
	///	@return		unsigned long
	/// @retval		0					No file is opened at the moment.
	///
	virtual unsigned	IMPEXP long GetMinorversion()=0;

    
	///
	///	@brief		Gets the framerate.
	///
	///				Will return the framerate at which the video has been recorded.
	///
	///	@return		double
	/// @retval		0					No file is opened at the moment.
	///
	virtual double		IMPEXP GetFrameRate()=0;

    
	///
	///	@brief		Gets the bitsize.
	///
	///				Will return the bitsize at which the video has been recorded.
	///
	///	@return		byte
	/// @retval		0					No file is opened at the moment.
	///
	virtual byte		IMPEXP GetBitSize()=0;

    
	///
	///	@brief		Gets the width.
	///
	///				Will return the width at which the video has been recorded.
	///
	///	@return		dword
	/// @retval		0					No file is opened at the moment.
	///
	virtual dword		IMPEXP GetWidth()=0;

    
	///
	///	@brief		Gets the height.
	///
	///				Will return the height at which the video has been recorded.
	///
	///	@return		dword
	/// @retval		0					No file is opened at the moment.
	///
	virtual dword		IMPEXP GetHeight()=0;

    
	///
	///	@brief		Gets the framesize.
	///
	///				Will return the size of one frame.
	///
	///	@return		dword
	/// @retval		0					No file is opened at the moment.
	///
	virtual dword		IMPEXP GetFrameSize()=0;

    
	///////////////////////////////////////////////////////////////////////////////////
	//	Zip Stream Properties.
    
	///
	///	@brief		Gets the stream entry count.
	///
	///				Will count the number of entries in the zip stream and returns the value.
	///
	///	@return		int
	/// @retval		0					No file is opened at the moment or this property is not supported for the given XVI File version
	///
	virtual int			IMPEXP GetEntryCount()=0;

    
	///
	///	@brief		Gets the entry name.
	///
	///				Gets the entry name for a given index and copies the value in entryName
	///
	///	@param[in]	index				Zero-based index of the entry.
	/// @param[out]	entryName			The returned entry name.
	/// @param[in]	maxLen				The maximum size of the char* buffer.
	///
	/// @return This method will return #I_OK on success.
    ///         On failure one of the other possible values is returned. (see #XviError)
    ///
	/// @retval		#E_NOT_SUPPORTED	This property is not supported for the given XVI file version.
	///	@retval		#E_FAIL				Could not initialize stream or there is no XVI opened.
	///	@retval		#E_OUT_OF_RANGE		The given index was not within the range of possible entries.
	///	@retval		#I_OK				The entryname is succesfully copied into the buffer.
	///
	virtual XviError	IMPEXP GetEntryName(unsigned int index, char *entryName, int maxLen)=0;

    
	///
	///	@brief		Gets the entry size.
	///
	///				Gets the entry size for the given entryName and sets the size parameter.
	///
	///	@param[in]  entryName			Name of the entry.
	/// @param[out]	size				The returned entry size.
	///
	/// @return This method will return #I_OK on success.
    ///         On failure one of the other possible values is returned. (see #XviError)
    ///
	/// @retval		#E_NOT_SUPPORTED	This property is not supported for the given XVI file version.
	///	@retval		#E_FAIL				Could not initialize stream or there is no XVI opened.
	///	@retval		#E_NOT_FOUND		The given entryName was not found within the stream entries.
	///	@retval		#I_OK				The entry size is successfully set into the size parameter.
	///
	virtual XviError	IMPEXP GetEntrySize(const char* entryName, int& size)=0;

    
	///
	///	@brief		Gets the entry value.
	///
	///				Gets the entry value for the given entryName and copies the value into the buffer.
	///
	///	@param[in]  entryName			Name of the entry.
	/// @param[out]	value				The returned entry value.
	/// @param[in]  maxLen				The maximum size of the char* buffer.
	///
	/// @return This method will return #I_OK on success.
    ///         On failure one of the other possible values is returned. (see #XviError)
    ///
	/// @retval		#E_NOT_SUPPORTED	This property is not supported for the given XVI file version.
	///	@retval		#E_FAIL				Could not initialize stream or there is no XVI opened.
	///	@retval		#E_NOT_FOUND		The given entryName was not found within the stream entries.
	/// @retval		#E_WRONG_BUFFERSIZE	The given buffer is too small.
	///	@retval		#I_OK				The value is successfully copied into the buffer.
	///
	virtual XviError	IMPEXP GetEntryValue(const char* entryName, char *value, int maxLen)=0;

    
    ///////////////////////////////////////////////////////////////////////////////////
	// Camera Settings Properties
    
	///
	///	@brief		Get the number of properties.
	///
	///				Gets the number of properties found inside the recsettings.xcf stream-entry.
	///
	/// @remark		In lower versions of Xeneth, the settings file located inside the stream was named settings.xcf
	///
	///	@return		int
	///	@retval		0					No file is opened at the moment or this property is not supported for the given XVI File version.
	///
	virtual int			IMPEXP GetPropertyCount()=0;

    
	///
	///	@brief		Gets the property name.
	///
	///				Gets the property name at a given index.
	///
	///	@param[in]  index				Zero-based index of the property.
	///	@param[out]	propertyName		The returned propertyName.
	///	@param[in]  maxLen				The length of the given buffer.
    ///
    /// @return This method will return #I_OK on success.
    ///         On failure one of the other possible values is returned. (see #XviError)
    ///
	///	@retval		#I_OK				The property name is successfully copied into the propertyName
	///
	virtual XviError	IMPEXP GetPropertyName(unsigned int index, char *propertyName, int maxLen)=0;

    
	///
	///	@brief		Gets the property value.
	///
	///				Gets the value of a given propertyName and copies it into the value.
	///
	///	@param[in]  propertyName		Name of the property.
	///	@param[out]	value				The returned value.
	///	@param[in]  maxLen				The length of the given buffer.
	///
    /// @return This method will return #I_OK on success.
    ///         On failure one of the other possible values is returned. (see #XviError)
    ///
	///	@retval		#I_OK				The value is successfully copied.
	virtual XviError	IMPEXP GetPropertyValue(const char *propertyName, char *value, int maxLen)=0;

    
	///
	///	@brief		Gets the property value.
	///
	///				Gets the value of a given propertyName and copies it into the value.
	///
	///	@param[in]  propertyName		Name of the property.
	///	@param[out]	value				The returned value.
	///
	/// @return This method will return #I_OK on success.
    ///         On failure one of the other possible values is returned. (see #XviError)
    ///
	///	@retval		#I_OK				The value is successfully copied.
    ///
	virtual XviError	IMPEXP GetPropertyValueL(const char *propertyName, long *value)=0;

    
	///
	///	@brief		Gets the property value.
	///
	///				Gets the value of a given propertyName and copies it into the value.
	///
	///	@param[in]  propertyName		Name of the property.
	///	@param[out]	value				The returned value.
	///
	/// @return This method will return #I_OK on success.
    ///         On failure one of the other possible values is returned. (see #XviError)
    ///
	///	@retval		#I_OK				The value is successfully copied into the given value parameter.
	///
    virtual XviError	IMPEXP GetPropertyValueF(const char *propertyName, double *value)=0;

    
    ///////////////////////////////////////////////////////////////////////////////////
	//	Frame data methods.
    
	///
	///	@brief		Gets a frame.
	///
	///				Gets a frame at the given index and copies it's value into the buffer.
	///
	///	@param[in]  frameType			The FrameType to which the frame has to be converted.
	/// @param[in]  ulFrameIdx			The zero-based index of the requested frame.
	/// @param[out]	buffer				The given buffer to which the frame has to be copied.
	/// @param[in]  size				The maximum size of the buffer.
	///
	/// @return This method will return #I_OK on success.
    ///         On failure one of the other possible values is returned. (see #XviError)
    ///
	///	@retval		#I_OK				Frame acquisition and conversion ok, the frame is copied into the buffer.
	///
	virtual XviError	IMPEXP GetFrame(FrameType frameType, unsigned long ulFrameIdx, void *buffer, unsigned int size)=0;

    
	///////////////////////////////////////////////////////////////////////////////////
	//	Utility methods.
    
	///
    ///	@brief		Gets the framecount
	///
	///				Gets the number of frames inside the opened XVI file.
	///
	///	@return		dword
	///	@retval		0					There's no XVI currently opened or no frames are available.
	///
	virtual dword		IMPEXP GetFrameCount()=0;
};

#endif

#ifdef __cplusplus
extern "C"
{
#endif

	///////////////////////////////////////////////////////////////////////////////////
	//	C Functions Definition
	///////////////////////////////////////////////////////////////////////////////////
	/// @defgroup	c_xvi	XVI C Functions
	///
	///	@brief		These are the wrapper methods for use in C applications.
	///
	///				These methods have the same functionality as the XviReader methods with the only difference	that a XVIHANDLE is used to keep a reference to the object.
	///				Such XVIHANDLE is initialized when the XVI_Open method is successfully called.
	///
	///	@{
	///////////////////////////////////////////////////////////////////////////////////
    //
	//	Type definitions
    //
    
	typedef int		XVIHANDLE;  ///< Handle to a XVI reader. (used in the C API functions)

	///////////////////////////////////////////////////////////////////////////////////
	//	Basic operations
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Open an XVI file.
	///
	///	@param[out]	h					Handle to an opened XVI file.
	///	@param[in]  filename			The file to be opened.
	///
	///	@remark		The only difference with the XviReader::Open method is that the XVIHANDLE
	///				parameter will be returned with a handle to the opened file, if operation was successful.
	///
	///	@see		XviReader::Open
	///
	XviError		IMPEXPC	XVI_Open			(XVIHANDLE *h, const char* filename);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Close the specified open XVI file.
	///
	///	@see		XviReader::Close
	///
	XviError		IMPEXPC	XVI_Close			(XVIHANDLE h);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Check if a file is currently opened
	///
	///	@see		XviReader::IsOpened
	///
	boole			IMPEXPC	XVI_IsOpened		(XVIHANDLE h);

    
	///////////////////////////////////////////////////////////////////////////////////
	//	Header Area 1.1
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the major version.
	///
	///	@see		XviReader::GetMajorversion
	///
	unsigned long	IMPEXPC	XVI_GetMajorversion	(XVIHANDLE h);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the minor version.
	///
	///	@see		XviReader::GetMinorversion
	///
	unsigned long	IMPEXPC	XVI_GetMinorversion	(XVIHANDLE h);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the frame rate.
	///
	///	@see		XviReader::GetFrameRate
	///
	double			IMPEXPC	XVI_GetFrameRate	(XVIHANDLE h);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the bit size.
	///
	///	@see		XviReader::GetBitSize
	///
	byte			IMPEXPC	XVI_GetBitSize		(XVIHANDLE h);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the width.
	///
	///	@see		XviReader::GetWidth
	///
	dword			IMPEXPC	XVI_GetWidth		(XVIHANDLE h);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the height.
	///
	///	@see		XviReader::GetHeight
	///
	dword			IMPEXPC	XVI_GetHeight		(XVIHANDLE h);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the framesize.
	///
	///	@see		XviReader::GetFrameSize
	///
	dword			IMPEXPC	XVI_GetFrameSize	(XVIHANDLE h);

    
	///////////////////////////////////////////////////////////////////////////////////
	//	Zip Stream Properties.
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the stream entry count.
	///
	///	@see		XviReader::GetEntryCount
	///
	int				IMPEXPC	XVI_GetEntryCount	(XVIHANDLE h);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the entry name.
	///
	///	@see		XviReader::GetEntryName
	///
	XviError		IMPEXPC	XVI_GetEntryName	(XVIHANDLE h, int index, char *entryName, int maxLen);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the entry size.
	///
	///	@see		XviReader::GetEntrySize
	///
	XviError		IMPEXPC	XVI_GetEntrySize	(XVIHANDLE h, const char* entryName, int *size);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the entry value.
	///
	///	@see		XviReader::GetEntryValue
	///
	XviError		IMPEXPC	XVI_GetEntryValue	(XVIHANDLE h, const char* entryName, char *value, int maxLen);

    
	///////////////////////////////////////////////////////////////////////////////////
	//	Camera Settings Properties.
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Get the number of properties.
	///
	///	@see		XviReader::GetPropertyCount
	///
	int				IMPEXPC	XVI_GetPropertyCount	(XVIHANDLE h);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the property name.
	///
	///	@see		XviReader::GetPropertyName
	///
	XviError		IMPEXPC	XVI_GetPropertyName	(XVIHANDLE h, int index, char *propertyName, int maxLen);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the property value.
	///
	///	@see		XviReader::GetPropertyValue
	///
	XviError		IMPEXPC	XVI_GetPropertyValue	(XVIHANDLE h, const char *propertyName, char *value, int maxlen);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the property value.
	///
	///	@see		XviReader::GetPropertyValueL
	///
	XviError		IMPEXPC	XVI_GetPropertyValueL	(XVIHANDLE h, const char *propertyName, long *value);

    
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the property value.
	///
	///	@see		XviReader::GetPropertyValueF
	///
	XviError		IMPEXPC	XVI_GetPropertyValueF	(XVIHANDLE h, const char *propertyName, double *value);

    
	///////////////////////////////////////////////////////////////////////////////////
	//	Frame data methods.
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets a frame.
	///
	///	@see		XviReader::GetFrame
	///
	XviError		IMPEXPC	XVI_GetFrame	(XVIHANDLE h, FrameType frameType, unsigned long ulFrameIdx, void *buffer, unsigned int size);

    
	///////////////////////////////////////////////////////////////////////////////////
	//	Utility methods.
	///////////////////////////////////////////////////////////////////////////////////
	///	@brief		Gets the framecount.
	///
	///	@see		XviReader::GetFrameCount
	///
	dword			IMPEXPC	XVI_GetFrameCount	(XVIHANDLE h);

	///////////////////////////////////////////////////////////////////////////////////
	/// @}
	///////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif


#endif // _XENICS_XVI_SDK_