/* ***************************************************************** */
/*                                                                   */
/* IBM Confidential                                                  */
/* OCO Source Materials                                              */
/*                                                                   */
/* (C) Copyright IBM Corp. 2001, 2014                                */
/*                                                                   */
/* The source code for this program is not published or otherwise    */
/* divested of its trade secrets, irrespective of what has been      */
/* deposited with the U.S. Copyright Office.                         */
/*                                                                   */
/* ***************************************************************** */

#ifdef _WIN32

//! Disable this warning as it incorrectly warns over template classes like std::list<>
#pragma warning( disable:4251) // warning C4251: ... needs to have dll-interface to be used by clients of class ...

#undef DLL
#ifdef SELF_LIB_EXPORTS
#define SELF_API __declspec(dllexport)
#else
#define SELF_API __declspec(dllimport)
#endif

#else

// Linux, Android and other platforms don't need to export from a .SO
#define SELF_API

#endif
