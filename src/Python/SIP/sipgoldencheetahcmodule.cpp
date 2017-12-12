/*
 * Module code.
 *
 * Generated by SIP 4.19.6
 */

#include "sipAPIgoldencheetah.h"

#line 59 "goldencheetah.sip"
#include "Bindings.h"
#line 12 "./sipgoldencheetahcmodule.cpp"
#line 110 "goldencheetah.sip"
//#include "Bindings.h"
#line 15 "./sipgoldencheetahcmodule.cpp"

/* Define the strings used by this module. */
const char sipStrings_goldencheetah[] = {
    'P', 'y', 't', 'h', 'o', 'n', 'D', 'a', 't', 'a', 'S', 'e', 'r', 'i', 'e', 's', 0,
    'a', 'c', 't', 'i', 'v', 'i', 't', 'y', 'M', 'e', 't', 'r', 'i', 'c', 's', 0,
    's', 'e', 'r', 'i', 'e', 's', 'P', 'r', 'e', 's', 'e', 'n', 't', 0,
    'g', 'o', 'l', 'd', 'e', 'n', 'c', 'h', 'e', 'e', 't', 'a', 'h', 0,
    '_', '_', 'g', 'e', 't', 'i', 't', 'e', 'm', '_', '_', 0,
    's', 'e', 'r', 'i', 'e', 's', 'L', 'a', 's', 't', 0,
    's', 'e', 'r', 'i', 'e', 's', 'N', 'a', 'm', 'e', 0,
    't', 'h', 'r', 'e', 'a', 'd', 'i', 'd', 0,
    'B', 'i', 'n', 'd', 'i', 'n', 'g', 's', 0,
    'w', 'e', 'b', 'p', 'a', 'g', 'e', 0,
    'v', 'e', 'r', 's', 'i', 'o', 'n', 0,
    'a', 't', 'h', 'l', 'e', 't', 'e', 0,
    '_', '_', 'l', 'e', 'n', '_', '_', 0,
    '_', '_', 's', 't', 'r', '_', '_', 0,
    'Q', 'S', 't', 'r', 'i', 'n', 'g', 0,
    's', 'e', 'r', 'i', 'e', 's', 0,
    'b', 'u', 'i', 'l', 'd', 0,
};


/*
 * This defines each type in this module.
 */
sipTypeDef *sipExportedTypes_goldencheetah[] = {
    &sipTypeDef_goldencheetah_Bindings.ctd_base,
    &sipTypeDef_goldencheetah_PythonDataSeries.ctd_base,
    &sipTypeDef_goldencheetah_QString.mtd_base,
};


/* This defines this module. */
sipExportedModuleDef sipModuleAPI_goldencheetah = {
    0,
    SIP_API_MINOR_NR,
    sipNameNr_goldencheetah,
    0,
    sipStrings_goldencheetah,
    NULL,
    NULL,
    3,
    sipExportedTypes_goldencheetah,
    NULL,
    0,
    NULL,
    0,
    NULL,
    NULL,
    NULL,
    {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};


/* The SIP API and the APIs of any imported modules. */
const sipAPIDef *sipAPI_goldencheetah;


/* The Python module initialisation function. */
#if PY_MAJOR_VERSION >= 3
#define SIP_MODULE_ENTRY        PyInit_goldencheetah
#define SIP_MODULE_TYPE         PyObject *
#define SIP_MODULE_DISCARD(r)   Py_DECREF(r)
#define SIP_MODULE_RETURN(r)    return (r)
#else
#define SIP_MODULE_ENTRY        initgoldencheetah
#define SIP_MODULE_TYPE         void
#define SIP_MODULE_DISCARD(r)
#define SIP_MODULE_RETURN(r)    return
#endif

#if defined(SIP_STATIC_MODULE)
extern "C" SIP_MODULE_TYPE SIP_MODULE_ENTRY()
#else
PyMODINIT_FUNC SIP_MODULE_ENTRY()
#endif
{
    static PyMethodDef sip_methods[] = {
        {0, 0, 0, 0}
    };

#if PY_MAJOR_VERSION >= 3
    static PyModuleDef sip_module_def = {
        PyModuleDef_HEAD_INIT,
        "goldencheetah",
        NULL,
        -1,
        sip_methods,
        NULL,
        NULL,
        NULL,
        NULL
    };
#endif

    PyObject *sipModule, *sipModuleDict;
    PyObject *sip_sipmod, *sip_capiobj;

    /* Initialise the module and get it's dictionary. */
#if PY_MAJOR_VERSION >= 3
    sipModule = PyModule_Create(&sip_module_def);
#elif PY_VERSION_HEX >= 0x02050000
    sipModule = Py_InitModule(sipName_goldencheetah, sip_methods);
#else
    sipModule = Py_InitModule(const_cast<char *>(sipName_goldencheetah), sip_methods);
#endif

    if (sipModule == NULL)
        SIP_MODULE_RETURN(NULL);

    sipModuleDict = PyModule_GetDict(sipModule);

    /* Get the SIP module's API. */
#if PY_VERSION_HEX >= 0x02050000
    sip_sipmod = PyImport_ImportModule(SIP_MODULE_NAME);
#else
    sip_sipmod = PyImport_ImportModule(const_cast<char *>(SIP_MODULE_NAME));
#endif

    if (sip_sipmod == NULL)
    {
        SIP_MODULE_DISCARD(sipModule);
        SIP_MODULE_RETURN(NULL);
    }

    sip_capiobj = PyDict_GetItemString(PyModule_GetDict(sip_sipmod), "_C_API");
    Py_DECREF(sip_sipmod);

#if defined(SIP_USE_PYCAPSULE)
    if (sip_capiobj == NULL || !PyCapsule_CheckExact(sip_capiobj))
#else
    if (sip_capiobj == NULL || !PyCObject_Check(sip_capiobj))
#endif
    {
        SIP_MODULE_DISCARD(sipModule);
        SIP_MODULE_RETURN(NULL);
    }

#if defined(SIP_USE_PYCAPSULE)
    sipAPI_goldencheetah = reinterpret_cast<const sipAPIDef *>(PyCapsule_GetPointer(sip_capiobj, SIP_MODULE_NAME "._C_API"));
#else
    sipAPI_goldencheetah = reinterpret_cast<const sipAPIDef *>(PyCObject_AsVoidPtr(sip_capiobj));
#endif

#if defined(SIP_USE_PYCAPSULE)
    if (sipAPI_goldencheetah == NULL)
    {
        SIP_MODULE_DISCARD(sipModule);
        SIP_MODULE_RETURN(NULL);
    }
#endif

    /* Export the module and publish it's API. */
    if (sipExportModule(&sipModuleAPI_goldencheetah,SIP_API_MAJOR_NR,SIP_API_MINOR_NR,0) < 0)
    {
        SIP_MODULE_DISCARD(sipModule);
        SIP_MODULE_RETURN(0);
    }
    /* Initialise the module now all its dependencies have been set up. */
    if (sipInitModule(&sipModuleAPI_goldencheetah,sipModuleDict) < 0)
    {
        SIP_MODULE_DISCARD(sipModule);
        SIP_MODULE_RETURN(0);
    }

    SIP_MODULE_RETURN(sipModule);
}
