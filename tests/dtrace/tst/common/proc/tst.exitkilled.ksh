#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"%Z%%M%	%I%	%E% SMI"

#
# This script tests that the proc:::exit probe fires with the correct argument
# when the process is killed.
#
# If this fails, the script will run indefinitely; it relies on the harness
# to time it out.
#
script()
{
	(cat <<EOF
	proc:::exit
	/curpsinfo->pr_ppid == $child &&
	    curpsinfo->pr_psargs == "$longsleep" && args[0] == CLD_KILLED/
	{
		exit(0);
	}
EOF
) > /tmp/dtest.$$
$dtrace -s /tmp/dtest.$$
}

sleeper()
{
	while true; do
		$longsleep &
		/usr/bin/sleep 1
		kill -9 $!
	done
}

if [ $# != 1 ]; then
	echo expected one argument: '<'dtrace-path'>'
	exit 2
fi

dtrace=$1
longsleep="/usr/bin/sleep 10000"

sleeper &
child=$!

script
status=$?

pstop $child
pkill -P $child
kill $child
prun $child

exit $status
