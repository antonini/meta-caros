SUMMARY = "D-Bus message bus"
DESCRIPTION = "D-Bus is a message bus system, a simple way for applications to talk to one another. In addition to interprocess communication, D-Bus helps coordinate process lifecycle; it makes it simple and reliable to code a \"single instance\" application or daemon, and to launch applications and daemons on demand when their services are needed."
HOMEPAGE = "http://dbus.freedesktop.org"
SECTION = "base"
LICENSE = "AFL-2 | GPLv2+"
LIC_FILES_CHKSUM = "file://COPYING;md5=10dded3b58148f3f1fd804b26354af3e \
                    file://dbus/dbus.h;beginline=6;endline=20;md5=7755c9d7abccd5dbd25a6a974538bb3c"
X11DEPENDS = "virtual/libx11 libsm"
DEPENDS = "expat virtual/libintl ${@base_contains('DISTRO_FEATURES', 'x11', '${X11DEPENDS}', '', d)}"
DEPENDS_class-native = "expat-native virtual/libintl-native"
DEPENDS_class-nativesdk = "nativesdk-expat virtual/nativesdk-libintl virtual/libx11"
RDEPENDS_dbus = "${@base_contains('DISTRO_FEATURES', 'ptest', 'dbus-ptest', '', d)}"
RDEPENDS_class-native = ""

INC_PR = "r5"

SRC_URI = "http://dbus.freedesktop.org/releases/dbus/dbus-${PV}.tar.gz \
           file://tmpdir.patch; \
           file://dbus-1.init"

inherit useradd autotools pkgconfig gettext update-rc.d

INITSCRIPT_NAME = "dbus-1"
INITSCRIPT_PARAMS = "start 02 5 3 2 . stop 20 0 1 6 ."

USERADD_PACKAGES = "${PN}"
GROUPADD_PARAM_${PN} = "-r netdev"
USERADD_PARAM_${PN} = "--system --home ${localstatedir}/lib/dbus \
                       --no-create-home --shell /bin/false \
                       --user-group messagebus"

CONFFILES_${PN} = "${sysconfdir}/dbus-1/system.conf ${sysconfdir}/dbus-1/session.conf"

DEBIANNAME_${PN} = "dbus-1"

PACKAGES =+ "${PN}-lib"

OLDPKGNAME = "dbus-x11"
OLDPKGNAME_class-nativesdk = ""

# for compatibility
RPROVIDES_${PN} = "${OLDPKGNAME}"
RREPLACES_${PN} += "${OLDPKGNAME}"

FILES_${PN} = "${bindir}/dbus-daemon* \
               ${bindir}/dbus-uuidgen \
               ${bindir}/dbus-cleanup-sockets \
               ${bindir}/dbus-send \
               ${bindir}/dbus-monitor \
               ${bindir}/dbus-launch \
               ${libexecdir}/dbus* \
               ${sysconfdir} \
               ${localstatedir} \
               ${datadir}/dbus-1/services \
               ${datadir}/dbus-1/system-services \
               ${systemd_unitdir}/system/"
FILES_${PN}-lib = "${libdir}/lib*.so.*"
RRECOMMENDS_${PN}-lib = "${PN}"
FILES_${PN}-dev += "${libdir}/dbus-1.0/include ${bindir}/dbus-glib-tool"

pkg_postinst_dbus() {
	if [ -z "$D" ] && [ -e /etc/init.d/populate-volatile.sh ] ; then
		/etc/init.d/populate-volatile.sh update
	fi
}

EXTRA_OECONF_X = "${@base_contains('DISTRO_FEATURES', 'x11', '--with-x', '--without-x', d)}"
EXTRA_OECONF_X_class-native = "--without-x --disable-x11-autolaunch"
# TODO: would like to --enable-systemd but that's a circular build-dependency
# between systemd <->dbus
EXTRA_OECONF_SYSTEMD = "${@base_contains('DISTRO_FEATURES', 'systemd', '--with-systemdsystemunitdir=${systemd_unitdir}/system/', '', d)}"

EXTRA_OECONF = "--disable-tests \
                --disable-checks \
                --disable-xml-docs \
                --disable-doxygen-docs \
                --disable-libaudit \
                --with-xml=expat \
                --disable-systemd \
                ${EXTRA_OECONF_SYSTEMD} \
                ${EXTRA_OECONF_X}"

do_install() {
	autotools_do_install

	if ${@base_contains('DISTRO_FEATURES', 'sysvinit', 'true', 'false', d)}; then
		install -d ${D}${sysconfdir}/init.d
		sed 's:@bindir@:${bindir}:' < ${WORKDIR}/dbus-1.init >${WORKDIR}/dbus-1.init.sh
		install -m 0755 ${WORKDIR}/dbus-1.init.sh ${D}${sysconfdir}/init.d/dbus-1
	fi

	install -d ${D}${sysconfdir}/default/volatiles
	echo "d messagebus messagebus 0755 ${localstatedir}/run/dbus none" \
	     > ${D}${sysconfdir}/default/volatiles/99_dbus


	mkdir -p ${D}${localstatedir}/run/dbus ${D}${localstatedir}/lib/dbus

	chown messagebus:messagebus ${D}${localstatedir}/run/dbus ${D}${localstatedir}/lib/dbus

	chown root:messagebus ${D}${libexecdir}/dbus-daemon-launch-helper
	chmod 4755 ${D}${libexecdir}/dbus-daemon-launch-helper

	# Remove Red Hat initscript
	rm -rf ${D}${sysconfdir}/rc.d

	# Remove empty testexec directory as we don't build tests
	rm -rf ${D}${libdir}/dbus-1.0/test
}

do_install_class-native() {
	autotools_do_install

	# for dbus-glib-native introspection generation
	install -d ${STAGING_DATADIR_NATIVE}/dbus/
	# N.B. is below install actually required?
	install -m 0644 bus/session.conf ${STAGING_DATADIR_NATIVE}/dbus/session.conf

	# dbus-glib-native and dbus-glib need this xml file
	./bus/dbus-daemon --introspect > ${STAGING_DATADIR_NATIVE}/dbus/dbus-bus-introspect.xml
}

do_install_class-nativesdk() {
	autotools_do_install
}
BBCLASSEXTEND = "native nativesdk"