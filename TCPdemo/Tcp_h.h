

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for Tcp.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __Tcp_h_h__
#define __Tcp_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITcp_FWD_DEFINED__
#define __ITcp_FWD_DEFINED__
typedef interface ITcp ITcp;

#endif 	/* __ITcp_FWD_DEFINED__ */


#ifndef __Tcp_FWD_DEFINED__
#define __Tcp_FWD_DEFINED__

#ifdef __cplusplus
typedef class Tcp Tcp;
#else
typedef struct Tcp Tcp;
#endif /* __cplusplus */

#endif 	/* __Tcp_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Tcp_LIBRARY_DEFINED__
#define __Tcp_LIBRARY_DEFINED__

/* library Tcp */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_Tcp,0x97C63F7A,0x17A6,0x40AD,0xB2,0xD3,0xAD,0xBA,0x4B,0x3D,0x67,0x23);

#ifndef __ITcp_DISPINTERFACE_DEFINED__
#define __ITcp_DISPINTERFACE_DEFINED__

/* dispinterface ITcp */
/* [uuid] */ 


DEFINE_GUID(DIID_ITcp,0xED2CCD13,0x959D,0x4741,0xA4,0x11,0x72,0x4B,0xFB,0x8A,0xAB,0x36);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("ED2CCD13-959D-4741-A411-724BFB8AAB36")
    ITcp : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ITcpVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITcp * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITcp * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITcp * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITcp * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITcp * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITcp * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITcp * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } ITcpVtbl;

    interface ITcp
    {
        CONST_VTBL struct ITcpVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITcp_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITcp_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITcp_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITcp_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITcp_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITcp_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITcp_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ITcp_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Tcp,0x048872F5,0xB529,0x4615,0x9C,0xD2,0x49,0xE0,0x9A,0x12,0xA7,0xCE);

#ifdef __cplusplus

class DECLSPEC_UUID("048872F5-B529-4615-9CD2-49E09A12A7CE")
Tcp;
#endif
#endif /* __Tcp_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


