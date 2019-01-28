Summary: The ClanLib Game SDK
Name: ClanLib
Version: 0.6.3
Release: 1
Copyright: LGPL
Group: System Environment/Libraries
BuildRoot: %{_tmppath}/%{name}-buildroot
Source: http://dark.x.dtu.dk/~mbn/clanlib/download/%{name}-%{version}.tar.bz2
URL: http://clanlib.org
Packager: Jeroen Janssen <japj@clanlib.org>
Requires: Hermes >= 1.3.0, zlib
BuildRequires: Hermes-devel >= 1.3.0 zlib-devel mikmod libpng-devel Mesa-devel vorbis libvorbis-devel
BuildConflicts: OpenPTC-devel
Prefix: %{_prefix}
ExclusiveArch: i386 i486 i586 i686

%description
The ClanLib Game SDK is a crossplatform game library designed to ease the work
for game developers. The goal is to provide a common interface to classical
game problems (loading graphics eg.), so games can share as much code as
possible. Ideally anyone with small resources should be able to write
commercial quality games.

%package gl
Summary: ClanLib OpenGL module
Group: System Environment/Libraries
Requires: ClanLib = %{version} Mesa
%description gl
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package is the OpenGL module (clanGL).

#%package magick
#Summary: ClanLib ImageMagick module
#Group: System Environment/Libraries
#Requires: ClanLib = %{version} ImageMagick
#%description magick
#The ClanLib Game SDK is a crossplatform game library designed to ease the
#work for game developers. This package is the ImageMagick module
#(clanMagick).

%package mikmod
Summary: ClanLib MikMod module
Group: System Environment/Libraries
Requires: ClanLib = %{version} 
%description mikmod
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package is the MikMod module (clanMikMod).

%package vorbis
Summary: ClanLib Ogg/Vorbis module
Group: System Environment/Libraries
Requires: ClanLib = %{version} vorbis
%description vorbis
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers.
This package adds Ogg/Vorbis support to clanlib.
Ogg/Vorbis is a free audio compression algorithm.

%package devel
Summary: ClanLib include files for developers
Group: Development/Libraries
Requires: ClanLib = %{version}
%description devel
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package contains the header files needed to
build ClanLib applications.

%package docs
Summary: ClanLib documentation
Group: System Environment/Libraries
%description docs
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package contains the documentation.

#%package lua
#Summary: ClanLib LUA add-on
#Group: System Environment/Libraries
#%description lua
#LUA Add-On for ClanLib

%package gui
Summary: ClanLib GUI add-on
Group: System Environment/Libraries
%description gui
GUI Add-On for ClanLib

#%package mpeg
#Summary: MPEG add-on for ClanLib
#Group: System Environment/Libraries
#%description mpeg
#MPEG Add-On for ClanLib

%prep
%setup -q

# Some people use HORRIBLE editors!!!
# Includes *MUST* end with a newline!
# Fix this up
for i in `find . -name "*.h" -type f`; do echo >>$i; done

%build
autoconf
%ifarch %{ix86}
  %configure --enable-x11 --enable-fbdev --enable-opengl --disable-svgalib --enable-clansound --enable-network --enable-asm386 --enable-dyn --disable-debug
%else
  %configure --enable-x11 --disable-fbdev --enable-opengl --disable-svgalib --enable-clansound --enable-network --enable-dyn --disable-asm386 --disable-debug
%endif
perl -pi -e "s/-O3/$RPM_OPT_FLAGS/" Setup/Unix/Makefile.conf
make clanCore
make clanApp
make clanDisplay
make clanSound
make clanGL
make clanMikMod
make clanNetwork
make clanJPEG
make clanTTF
make clanSmallJPEG
#make clanMagick
make clanGUI
#make clanLua
#make clanMPEG
make clanPNG
make strip
make docs

%install
%makeinstall BIN_PREFIX=$RPM_BUILD_ROOT%{_bindir} LIB_PREFIX=$RPM_BUILD_ROOT%{_libdir} INC_PREFIX=$RPM_BUILD_ROOT%{_includedir} TARGET_PREFIX=$RPM_BUILD_ROOT%{_libdir}/%{name}
make MAN_PREFIX=$RPM_BUILD_ROOT%{_mandir} HTML_PREFIX=$RPM_BUILD_ROOT%{_datadir}/doc/%{name}-docs-%{version}/Docs docs_install

%post -p /sbin/ldconfig
%post gl -p /sbin/ldconfig
%post mikmod -p /sbin/ldconfig
%post vorbis -p /sbin/ldconfig
#%post magick -p /sbin/ldconfig
%post gui -p /sbin/ldconfig

%postun -p /sbin/ldconfig
%postun	gl -p /sbin/ldconfig
%postun	mikmod -p /sbin/ldconfig
%postun	vorbis -p /sbin/ldconfig
#%postun magick -p /sbin/ldconfig
%postun gui -p /sbin/ldconfig

%clean
#rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
%doc README COPYING CREDITS CODING_STYLE ascii-logo
%{_libdir}/libclanCore.so.*
%{_libdir}/libclanApp.so.*
%{_libdir}/libclanDisplay.so.*
%{_libdir}/libclanJPEG.so.*
%{_libdir}/libclanPNG.so.*
%{_libdir}/libclanSound.so.*
%{_libdir}/libclanNetwork.so.*

%files devel
%defattr(-, root, root)
%{_libdir}/*.so.*
%{_includedir}/ClanLib/*
%{_bindir}/*

%files docs
%defattr(-, root, root)
%{_docdir}/%{name}-docs-%{version}/*

%files gl
%defattr(-, root, root)
%{_libdir}/libclanGL.so.*

#%files magick
#%defattr(-, root, root)
#%{_libdir}/libclanMagick.so*

%files mikmod
%defattr(-, root, root)
%{_libdir}/libclanMikMod.so.*

%files vorbis
%defattr(-, root, root)
%{_libdir}/libclanVorbis.so.*

#%files lua
#%defattr(-, root, root)
#%{prefix}/lib/libclanLua.so*

%files gui
%defattr(-, root, root)
%{prefix}/lib/libclanGUI.so.*

#%files mpeg
#%defattr(-, root, root)
#%{prefix}/lib/libclanMPEG.so*

%changelog
* Thu Mar 28 2002 Jeroen Janssen <japj@clanlib.org>
- Update to 0.6.0

* Fri Nov 02 2001 Jeroen Janssen <japj@clanlib.org>
- Update to 0.5.1

* Wed Nov 29 2000 Bernhard Rosenkraenzer <bero@redhat.com>
- Update to 0.5
- Fix up the include files and make a mental note to kill people
  who don't end headers with a newline...
- ExclusiveArch ix86, compiler problems

* Tue Nov 28 2000 Bernhard Rosenkraenzer <bero@redhat.com>
- ExtraBinge -> Powertools
- Fix a couple of bugs in the spec file
- Fix up the disabled modules, enable them
- Remove SVGALib support (incompatible)
- Fix build on non-x86

