/** @file
 *
 *  secure core interface.
 *
 *  @author     stan.kim@lge.com
 *  @version    2.0
 *  @date       2012.5
 *  @note       Additional information.
 *
 *	@{
 */

#ifndef	_SC_IF_H_
#define	_SC_IF_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

typedef enum {
	SC_IF_ERROR		= -1,
	SC_IF_OK		= 0,
	SC_IF_NO_AU		= 1,
	SC_IF_WAIT		= 2,
} SC_IF_RETURN_T;

typedef struct _SC_IF_RoundBuffer_tag {
	unsigned char *pBuffer;
	int size;
	int readOffset;
	int writeOffset;
    int emptyAUIBEntry;
} SC_IF_CPB_T;

typedef struct _SC_IF_AU_tag {
	int offset;
	int length;
	unsigned long long pts;
} SC_IF_AU_T;

typedef struct _SC_IF_tag {
	SC_IF_RETURN_T (*SC_IF_RunCommand)(int command);
	SC_IF_RETURN_T (*SC_IF_DecryptCW)(unsigned char *pCW, int cwLength);
	SC_IF_RETURN_T (*SC_IF_DecryptContent)(SC_IF_AU_T *pAu, SC_IF_CPB_T *pIn, SC_IF_CPB_T *pOut, int bInit);
} SC_IF_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/**
 *	Get Clock(uSec)
 *
 *	@return	uSec clock.
 */
extern long long SC_IF_Clock(void);

/**
 *	Register SC IF
 *
 *	@param	pScIf	[in]	SC IF Functions
 *	@return	SC_IF_OK : Success, SC_IF_ERROR : Failure.
 */
extern SC_IF_RETURN_T SC_IF_Init(SC_IF_T *pScIf);

/**
 *	SC Run Command.
 *
 *	@param	command		[in]	Command.
 *	@return	SC_IF_OK : Success, SC_IF_ERROR : Failure.
 */
extern SC_IF_RETURN_T SC_IF_RunCommand(int command);

/**
 *	SC Decrypt Control Word.
 *
 *	@param	pCW			[in/out]	Encrypted In/Decrypted Out.
 *	@param	cwLength	[in]		Control Word Length.
 *	@return	SC_IF_OK : Success, SC_IF_ERROR : Failure.
 */
extern SC_IF_RETURN_T SC_IF_DecryptCW(unsigned char *pCW, int cwLength);

/**
 *	SC Decyrpt PlayReady.
 *
 *	@param	pAu			[out]		AU Out.
 *	@param	pIn			[in/out]	Input circular buffer(PDEC CPB).
 *	@param	pOut		[in/out]	Output circular buffer(VDEC CPB).
 *	@return	SC_IF_ERROR : Failure.
 *			SC_IF_OK	: AU Out - pAuOffset and pAuLength are valid.
 *			SC_IF_NO_AU	: Not complete - pAuOffset and pAuLength are invalid.
 *			SC_IF_WAIT	: pOut buffer full(Wait and re-call this function).
 */
extern SC_IF_RETURN_T SC_IF_DecryptContent(SC_IF_AU_T *pAu, SC_IF_CPB_T *pIn, SC_IF_CPB_T *pOut, int bInit);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SC_IF_H_ */

/** @} */

