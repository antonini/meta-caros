DESCRIPTION = "A mocking library for Erlang"
SECTION = "devel"
LICENSE = "Apache-2.0"

LIC_FILES_CHKSUM = "file://LICENSE;md5=ff253ad767462c46be284da12dda33e8"

PR = "r1"

SRC_URI = "git://git@git.tpip.net:meck"
SRCREV = "0a31b90224a5abdf63f129402f255baa7f8ae329"

S = "${WORKDIR}/git"

inherit tetrapak

python () {
    erlang_def_package("meck", "meck-*", "ebin", "LICENSE CHANGELOG src Makefile test rebar.config README.md NOTICE doc .travis.yml .scripts", d)
}
