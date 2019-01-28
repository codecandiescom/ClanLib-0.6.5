%define name clanlib0.6
%define version	0.6.3
%define release 1mdk
%define prefix %{_prefix}
%define lib_name_orig libclanlib
%define lib_major 2
%define lib_name %{lib_name_orig}%{lib_major}

Name: %{name}
Summary: The ClanLib Game SDK
Version: %{version}
Release: %{release}
License: LGPL
Group: System/Libraries
Source: http://dark.x.dtu.dk/~mbn/clanlib/download/download-sphair/ClanLib-%{version}.tar.bz2
URL: http://clanlib.org/
Packager: Jeroen Janssen <japj@clanlib.org>
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
BuildRequires: libhermes-devel >= 1.3.0 libmikmod-devel libpng-devel Mesa-common-devel bzip2 libjpeg-devel libtiff-devel XFree86-static-libs libbzip2-devel libvorbis-devel libogg-devel freetype2-devel
BuildConflicts:	OpenPTC-devel
#%ifarch %{ix86}
#BuildRequires: svgalib-devel
#%endif
Obsoletes: ClanLib
Provides: ClanLib = %{version}-%{release}

%description
The ClanLib Game SDK is a crossplatform game library designed to ease the work
for game developers. The goal is to provide a common interface to classical
game problems (loading graphics eg.), so games can share as much code as
possible. Ideally anyone with small resources should be able to write
commercial quality games.

%package -n %{lib_name}
Summary: Main library for %{name}
Group: System/Libraries

%description -n %{lib_name}
This package contains the library needed to run programs dynamically
linked with %{name}.

%package -n %{lib_name}-devel
Summary: Headers for developing programs that will use %{name}
Group: Development/C++
Requires: %{lib_name} = %{version}
Provides: %{lib_name_orig}-devel = %{version}-%{release}
Obsoletes: ClanLib-devel clanlib-devel
Provides: ClanLib-devel = %{version}-%{release} clanlib-devel = %{version}-%{release}

%description -n %{lib_name}-devel
This package contains the headers that programmers will need to develop
applications which will use %{name}.


%package -n %{lib_name}-gl
Summary: ClanLib OpenGL module
Group: System/Libraries
Requires: %{lib_name} = %{version} Mesa
BuildRequires: Mesa-common-devel

%description -n %{lib_name}-gl
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package is the OpenGL module (clanGL).


%package -n %{lib_name}-sound
Summary: ClanLib Sound module
Group: System/Libraries
Requires: %{lib_name} = %{version}

%description -n %{lib_name}-sound
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package is the Sound module (clanSound).

%package -n %{lib_name}-vorbis
Summary: ClanLib Vorbis module
Group: System/Libraries
Requires: %{lib_name} = %{version}

%description -n %{lib_name}-vorbis
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package is the Vorbis module (clanVorbis).

%package -n %{lib_name}-network
Summary: ClanLib Network module
Group: System/Libraries
Requires: %{lib_name} = %{version}

%description -n %{lib_name}-network
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package is the Network module (clanNetwork).

%package -n %{lib_name}-jpeg
Summary: ClanLib Jpeg module
Group: System/Libraries
Requires: %{lib_name} = %{version}

%description -n %{lib_name}-jpeg
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package is the Jpeg module (clanJPEG).

%package -n %{lib_name}-gui
Summary: ClanLib Gui module
Group: System/Libraries
Requires: %{lib_name} = %{version}

%description -n %{lib_name}-gui
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package is the Gui module (clanGUI).

%package -n %{lib_name}-ttf
Summary: ClanLib TTF module
Group: System/Libraries
Requires: %{lib_name} = %{version}

%description -n %{lib_name}-ttf
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package is the TTF module (clanTTF).

%package -n %{lib_name}-smalljpeg
Summary: ClanLib SmallJPEG module
Group: System/Libraries
Requires: %{lib_name} = %{version}

%description -n %{lib_name}-smalljpeg
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package is the SmallJPEG module
(clanSmallJPEG).

%package -n %{lib_name}-mikmod
Summary: ClanLib MikMod module
Group: System/Libraries
Requires: %{lib_name} = %{version}
Obsoletes: ClanLib-mikmod clanlib-mikmod
Provides: ClanLib-mikmod = %{version}-%{release} clanlib-mikmod = %{version}-%{release}

%description -n %{lib_name}-mikmod
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package is the MikMod module (clanMikMod).

%package -n %{lib_name}-png
Summary: ClanLib PNG module
Group: System/Libraries
Requires: %{lib_name} = %{version}
Obsoletes: ClanLib-png clanlib-png
Provides: ClanLib-png = %{version}-%{release} clanlib-png = %{version}-%{release}

%description -n %{lib_name}-png
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package is the PNG module (clanPNG).

%package docs
Summary: ClanLib documentation
Group: System/Libraries
Obsoletes: ClanLib-docs clanlib-docs
Provides: ClanLib-docs = %{version}-%{release} clanlib-docs = %{version}-%{release}

%description docs
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. This package contains the documentation.

%prep
%setup -q -n ClanLib-%{version}

%build
# (sb) doesn't build on PPC with this
%ifnarch ppc
# (gc) workaround g++ exception bug when -fomit-frame-pointer is set
export CFLAGS="$RPM_OPT_FLAGS -fno-omit-frame-pointer" CXXFLAGS="$RPM_OPT_FLAGS -fno-omit-frame-pointer"
%endif
autoconf
%ifarch %{ix86}
  %configure --enable-dyn
%else
  # (sb) configure macro breaks PPC build - just do it manually for now
  %ifnarch ppc
    %configure --enable-dyn --disable-asm386
  %else
    libtoolize --copy --force
    ./configure --enable-dyn --disable-asm386  
  %endif
%endif
make all
make strip
make docs

%install
%makeinstall BIN_PREFIX=$RPM_BUILD_ROOT%{_bindir} LIB_PREFIX=$RPM_BUILD_ROOT%{_libdir} INC_PREFIX=$RPM_BUILD_ROOT%{_includedir} TARGET_PREFIX=$RPM_BUILD_ROOT%{_libdir}/ClanLib
make MAN_PREFIX=$RPM_BUILD_ROOT%{_mandir} HTML_PREFIX=$RPM_BUILD_ROOT%{_datadir}/doc/%{name}-docs-%{version}/Docs docs_install

%post -n %{lib_name} -p /sbin/ldconfig
%post -n %{lib_name}-gl -p /sbin/ldconfig
%post -n %{lib_name}-mikmod -p /sbin/ldconfig
%post -n %{lib_name}-png -p /sbin/ldconfig
%post -n %{lib_name}-sound -p /sbin/ldconfig
%post -n %{lib_name}-vorbis -p /sbin/ldconfig
%post -n %{lib_name}-network -p /sbin/ldconfig
%post -n %{lib_name}-jpeg -p /sbin/ldconfig
%post -n %{lib_name}-gui -p /sbin/ldconfig
%post -n %{lib_name}-ttf -p /sbin/ldconfig
%post -n %{lib_name}-smalljpeg -p /sbin/ldconfig

%postun -n %{lib_name} -p /sbin/ldconfig
%postun -n %{lib_name}-gl -p /sbin/ldconfig
%postun	-n %{lib_name}-mikmod -p /sbin/ldconfig
%postun	-n %{lib_name}-png -p /sbin/ldconfig
%postun -n %{lib_name}-sound -p /sbin/ldconfig
%postun -n %{lib_name}-vorbis -p /sbin/ldconfig
%postun -n %{lib_name}-network -p /sbin/ldconfig
%postun -n %{lib_name}-jpeg -p /sbin/ldconfig
%postun -n %{lib_name}-gui -p /sbin/ldconfig
%postun -n %{lib_name}-ttf -p /sbin/ldconfig
%postun -n %{lib_name}-smalljpeg -p /sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT

%files -n %{lib_name}
%defattr(-, root, root)
%doc README COPYING CREDITS
%{_libdir}/libclanCore.so.*
%{_libdir}/libclanApp.so.*
%{_libdir}/libclanDisplay.so.*

%files -n %{lib_name}-devel
%defattr(-, root, root)
%doc README COPYING CODING_STYLE ascii-logo NEWS
%{_libdir}/*.so
%{_includedir}/ClanLib/*
%{_bindir}/clanlib-config

%files docs
%defattr(-, root, root)
%{_docdir}/%{name}-docs-%{version}/*

%files -n %{lib_name}-gl
%defattr(-, root, root)
%doc README COPYING
%{_libdir}/libclanGL.so.*

%files -n %{lib_name}-mikmod
%defattr(-, root, root)
%doc README COPYING 
%{_libdir}/libclanMikMod.so.*

%files -n %{lib_name}-png
%defattr(-, root, root)
%doc README COPYING 
%{_libdir}/libclanPNG.so.*

%files -n %{lib_name}-ttf
%defattr(-, root, root)
%doc README COPYING 
%{_libdir}/libclanTTF.so.*

%files -n %{lib_name}-jpeg
%defattr(-, root, root)
%doc README COPYING 
%{_libdir}/libclanJPEG.so.*

%files -n %{lib_name}-network
%defattr(-, root, root)
%doc README COPYING 
%{_libdir}/libclanNetwork.so.*

%files -n %{lib_name}-vorbis
%defattr(-, root, root)
%doc README COPYING 
%{_libdir}/libclanVorbis.so.*

%files -n %{lib_name}-smalljpeg
%defattr(-, root, root)
%doc README COPYING 
%{_libdir}/libclanSmallJPEG.so.*

%files -n %{lib_name}-sound
%defattr(-, root, root)
%{prefix}/lib/libclanSound.so.*

%files -n %{lib_name}-gui
%defattr(-, root, root)
%{prefix}/lib/libclanGUI.so.*

#%files mpeg
#%defattr(-, root, root)
#%{prefix}/lib/libclanMPEG.so*

%changelog
* Thu Mar 28 2002 Jeroen Janssen <japj@clanlib.org>
- final update to 0.6.0

* Fri Jan 25 2002 Jeroen Janssen <japj@clanlib.org> 0.6.0
- initial 0.6.0 spec
