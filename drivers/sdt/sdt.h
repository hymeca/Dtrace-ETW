/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Copyright (c) 2012, Joyent, Inc. All rights reserved.
 * Copyright (C) 2019, PK.
 */

#ifndef _SYS_SDT_IMPL_H
#define	_SYS_SDT_IMPL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <sys/dtrace.h>

#if defined(__i386) || defined(__amd64)
typedef uint8_t sdt_instr_t;
#else
typedef uint32_t sdt_instr_t;
#endif

int sdt_attach();
int sdt_detach();

typedef struct sdt_etw_provider sdt_etw_provider_t;

typedef struct sdt_provider {
	char *sdtp_name;			/* name of provider */
	char *sdtp_prefix;			/* prefix for probe names */
	dtrace_pattr_t *sdtp_attr;		/* stability attributes */
	sdt_etw_provider_t *sdtp_etw;	/* etw events */
	uint64_t sdtp_etw_flags;	/* etw kernel prov enable flag or */
								/* etw prov keyword */
	uint32_t sdtp_priv;				/* privilege, if any */
	dtrace_provider_id_t sdtp_id;	/* provider ID */
} sdt_provider_t;

extern sdt_provider_t sdt_providers[];		/* array of providers */

typedef struct sdt_probe {
	sdt_provider_t	*sdp_provider;		/* provider */
	char		*sdp_name;		/* name of probe */
	int		sdp_namelen;		/* length of allocated name */
	dtrace_id_t	sdp_id;			/* probe ID */
	sdt_etw_provider_t	*sdp_ctl;		/* etw for probe */
	int		sdp_loadcnt;		/* load count for module */
	int		sdp_primary;		/* non-zero if primary mod */
	sdt_instr_t	*sdp_patchpoint;	/* patch point */
	sdt_instr_t	sdp_patchval;		/* instruction to patch */
	sdt_instr_t	sdp_savedval;		/* saved instruction value */
	struct sdt_probe *sdp_next;		/* next probe */
	struct sdt_probe *sdp_hashnext;		/* next on hash */
} sdt_probe_t;

typedef struct sdt_argdesc {
	const char *sda_provider;		/* provider for arg */
	const char *sda_name;			/* name of probe */
	const int sda_ndx;			/* argument index */
	const int sda_mapping;			/* mapping of argument */
	const char *sda_native;			/* native type of argument */
	const char *sda_xlate;			/* translated type of arg */
} sdt_argdesc_t;

struct sdt_etw_provider {
	char *etw_nprov;
	char *etw_name;
	const GUID *etw_guid;
	int etw_eventno;
	int (*etw_cb) (PEVENT_RECORD, void *);
	const GUID *etw_bguid;
	int (*etw_bcb) (PEVENT_RECORD, void *);
	uint64_t etw_kw;
};

extern void sdt_getargdesc(void *, dtrace_id_t, void *, dtrace_argdesc_t *);


#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_SDT_IMPL_H */
