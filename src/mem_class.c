/*
 * This file is part of esolid-mem
 *
 * Template version: 1.1.16 (24.12.2013)
 *
 * Copyright (C) 2011, 2012 - Nenad Radulovic
 *
 * esolid-mem is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * esolid-mem is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with eSolid.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://blueskynet.dyndns-server.com
 * e-mail  :    blueskyniss@gmail.com
 *//***********************************************************************//**
 * @file
 * @author      nenad
 * @brief       Short desciption of file
 * @addtogroup  module_impl
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include "plat/critical.h"
#include "mem/mem_class.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define MEM_SIGNATURE                   ((esAtomic)0x01010505ul)

#define MEM_CLASS_SIGNATURE             ((esAtomic)0x5a5a5a5aul)

/*======================================================  LOCAL DATA TYPES  ==*/

struct esMemClass {
    esError          (* init)    (void *, void *, size_t, size_t);
    esError          (* term)    (void *);
    esError          (* alloc)   (void *, size_t, void **);
    esError          (* free)    (void *, void *);
    esError          (* getSize) (void *, size_t *);
    esError          (* getFree) (void *, size_t *);
    esError          (* getBlockSize)(void *, size_t *);
#if (CONFIG_API_VALIDATION) || defined(__DOXYGEN__)
    esAtomic            signature;
#endif
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esError wrapHeapInit(
    void *              handle,
    void *              storage,
    size_t              storageSize,
    size_t              block);

static esError wrapPoolInit(
    void *              handle,
    void *              storage,
    size_t              storageSize,
    size_t              block);

static esError wrapStaticInit(
    void *              handle,
    void *              storage,
    size_t              storageSize,
    size_t              block);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("mem class", "Memory classes", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/

const PORT_C_ROM struct esMemClass esGlobalHeapMemClass = {
    wrapHeapInit,
    (esError (*)(void *))esHeapMemTerm,
    (esError (*)(void *, size_t, void **))esHeapMemAllocI,
    (esError (*)(void *, void *))esHeapMemFreeI,
    (esError (*)(void *, size_t *))esHeapGetSizeI,
    NULL,
    (esError (*)(void *, size_t *))esHeapGetBlockSizeI
#if (CONFIG_API_VALIDATION) || defined(__DOXYGEN__)
    , MEM_CLASS_SIGNATURE
#endif
};

const PORT_C_ROM struct esMemClass esGlobalPoolMemClass = {
    wrapPoolInit,
    NULL,
    (esError (*)(void *, size_t, void **))esPoolMemAllocI,
    (esError (*)(void *, void *))esPoolMemFreeI,
    NULL,
    NULL,
    NULL
#if (CONFIG_API_VALIDATION) || defined(__DOXYGEN__)
    , MEM_CLASS_SIGNATURE
#endif
};

const PORT_C_ROM struct esMemClass esGlobalStaticMemClass = {
    wrapStaticInit,
    NULL,
    (esError (*)(void *, size_t, void **))esStaticMemAllocI,
    NULL,
    (esError (*)(void *, size_t *))esStaticMemGetSizeI,
    NULL,
    NULL
#if (CONFIG_API_VALIDATION) || defined(__DOXYGEN__)
    , MEM_CLASS_SIGNATURE
#endif
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static esError wrapHeapInit(
    void *              handle,
    void *              storage,
    size_t              storageSize,
    size_t              block) {

    esError             error;

    (void)block;
    error = esHeapMemInit(handle, storage, storageSize);

    return (error);
}

static esError wrapPoolInit(
    void *              handle,
    void *              storage,
    size_t              storageSize,
    size_t              block) {

    esError             error;

    error = esPoolMemInit(handle, storage, storageSize, block);

    return (error);
}

static esError wrapStaticInit(
    void *              handle,
    void *              storage,
    size_t              storageSize,
    size_t              block) {

    esError             error;

    (void)block;
    error = esStaticMemInit(handle, storage, storageSize);

    return (error);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

esError esMemInit(
    const struct esMemClass * class,
    struct esMem *      object,
    void *              buffer,
    size_t              size,
    size_t              block) {

    esError             error;

    ES_API_REQUIRE(ES_API_POINTER, object != NULL);
    ES_API_REQUIRE(ES_API_OBJECT,  object->signature != MEM_SIGNATURE);
    ES_API_REQUIRE(ES_API_POINTER, class != NULL);
    ES_API_REQUIRE(ES_API_OBJECT,  class->signature == MEM_CLASS_SIGNATURE);
    ES_API_REQUIRE(ES_API_POINTER, buffer != NULL);

    error = (class->init)(&object->handle, buffer, size, block);

    if (error == ES_ERROR_NONE) {
        object->class = class;
        ES_API_OBLIGATION(object->signature = MEM_SIGNATURE);
    }

    return (error);
}

esError esMemTerm(
    struct esMem * object) {

    esError             error;

    ES_API_REQUIRE(ES_API_POINTER, object != NULL);
    ES_API_REQUIRE(ES_API_OBJECT,  object->signature == MEM_SIGNATURE);

    error = ES_ERROR_NOT_IMPLEMENTED;

    if (object->class->term != NULL) {
        error = (object->class->term)(&object->handle);
    }
    object->class = NULL;
    ES_API_OBLIGATION(object->signature = ~(esAtomic)MEM_SIGNATURE);

    return (error);
}

esError esMemAllocI(
    struct esMem *      object,
    size_t              size,
    void **             mem) {

    esError             error;

    ES_API_REQUIRE(ES_API_POINTER, object != NULL);
    ES_API_REQUIRE(ES_API_OBJECT,  object->signature == MEM_SIGNATURE);

    error = (object->class->alloc)(&object->handle, size, mem);

    return (error);
}

esError esMemAlloc(
    struct esMem *      object,
    size_t              size,
    void **             mem) {

    esError             error;
    esLockCtx           lockCtx;

    ES_CRITICAL_LOCK_ENTER(&lockCtx);
    error = esMemAllocI(object, size, mem);
    ES_CRITICAL_LOCK_EXIT(lockCtx);

    return (error);
}

esError esMemFreeI(
    struct esMem *      object,
    void *              mem) {

    esError             error;

    ES_API_REQUIRE(ES_API_POINTER, object != NULL);
    ES_API_REQUIRE(ES_API_OBJECT,  object->signature == MEM_SIGNATURE);

    error = ES_ERROR_NOT_IMPLEMENTED;

    if (object->class->free != NULL) {
        error = (object->class->free)(&object->handle, mem);
    }

    return (error);
}

esError esMemFree(
    struct esMem *      object,
    void *              mem) {

    esError             error;
    esLockCtx           lockCtx;

    ES_CRITICAL_LOCK_ENTER(&lockCtx);
    error = esMemFreeI(object, mem);
    ES_CRITICAL_LOCK_EXIT(lockCtx);

    return (error);
}

esError esMemGetSizeI(
    struct esMem *      object,
    size_t *            size) {

    esError             error;

    ES_API_REQUIRE(ES_API_POINTER, object != NULL);
    ES_API_REQUIRE(ES_API_OBJECT,  object->signature == MEM_SIGNATURE);

    error = ES_ERROR_NOT_IMPLEMENTED;

    if (object->class->getSize != NULL) {
        error = (object->class->getSize)(&object->handle, size);
    }

    return (error);
}

esError esMemGetSize(
    struct esMem *      object,
    size_t *            size) {

    esError             error;
    esLockCtx           lockCtx;

    ES_CRITICAL_LOCK_ENTER(&lockCtx);
    error = esMemGetSizeI(object, size);
    ES_CRITICAL_LOCK_EXIT(lockCtx);

    return (error);
}

esError esMemGetFreeI(
    struct esMem * object,
    size_t *            free) {

    esError             error;

    ES_API_REQUIRE(ES_API_POINTER, object != NULL);
    ES_API_REQUIRE(ES_API_OBJECT,  object->signature == MEM_SIGNATURE);

    error = ES_ERROR_NOT_IMPLEMENTED;

    if (object->class->getFree != NULL) {
        error = (object->class->getFree)(&object->handle, free);
    }

    return (error);

}

esError esMemGetFree(
    struct esMem *      object,
    size_t *            free) {

    esError             error;
    esLockCtx           lockCtx;

    ES_CRITICAL_LOCK_ENTER(&lockCtx);
    error = esMemGetFreeI(object, free);
    ES_CRITICAL_LOCK_EXIT(lockCtx);

    return (error);

}

esError esMemGetBlockSizeI(
    struct esMem *      object,
    size_t *            blockSize) {

    esError             error;

    ES_API_REQUIRE(ES_API_POINTER, object != NULL);
    ES_API_REQUIRE(ES_API_OBJECT,  object->signature == MEM_SIGNATURE);

    error = ES_ERROR_NOT_IMPLEMENTED;

    if (object->class->getBlockSize != NULL) {
        error = (object->class->getBlockSize)(&object->handle, blockSize);
    }

    return (error);
}

esError esMemGetBlockSize(
    struct esMem *      object,
    size_t *            blockSize) {

    esError             error;
    esLockCtx           lockCtx;

    ES_CRITICAL_LOCK_ENTER(&lockCtx);
    error = esMemGetBlockSizeI(object, blockSize);
    ES_CRITICAL_LOCK_EXIT(lockCtx);

    return (error);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mem_class.c
 ******************************************************************************/
