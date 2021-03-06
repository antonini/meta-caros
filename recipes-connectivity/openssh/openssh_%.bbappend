PR := "${PR}.1"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://etc_systemd_system_sshd.socket.d_override.conf"

do_install_append () {
    install -d ${D}${sysconfdir}/systemd/system/sshd.socket.d
    install -m 0644 ${WORKDIR}/etc_systemd_system_sshd.socket.d_override.conf ${D}${sysconfdir}/systemd/system/sshd.socket.d/override.conf
}

FILES_${PN}-sshd += "${sysconfdir}/systemd/system/sshd.socket.d/override.conf"
CONFFILES_${PN}-sshd += "${sysconfdir}/systemd/system/sshd.socket.d/override.conf"
