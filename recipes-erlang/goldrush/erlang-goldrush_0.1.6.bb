DESCRIPTION = "Small, Fast event processing and monitoring for Erlang/OTP applications."
SECTION = "devel"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=7b9f7f44f40906370137171b3437c818"

PR = "r4"

SRC_URI = "https://github.com/DeadZen/goldrush/archive/${PV}.tar.gz;downloadfilename=goldrush-${PV}.tar.gz"

SRC_URI[md5sum] = "119db963081dcadd51e9f8698b41c557"
SRC_URI[sha256sum] = "554f36983e19fcf2ebee18b5c0ff502af4fc1b853b49041c7a9821d7043e349f"

S = "${WORKDIR}/goldrush-${PV}"

inherit rebar
