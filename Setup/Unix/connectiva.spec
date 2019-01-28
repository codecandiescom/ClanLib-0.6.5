Name: ClanLib
Version: 0.5.0
Release: 10cl
Summary: The ClanLib Game SDK.
Summary(pt_BR): SDK Clanlib
Summary(es): The ClanLib Game SDK.
License: LGPL
Group: Development
Group(pt_BR): Desenvolvimento
Group(es): Desarrollo
# Sources grabbed from cvs
URL: http://clanlib.org/
Source: http://dark.x.dtu.dk/~mbn/clanlib/download/%{name}-%{version}.tar.bz2
Requires: Hermes >= 1.3.2, zlib
BuildRequires: ImageMagick-libMagick-devel, libbz2-devel
BuildRequires: Hermes-devel >= 1.3.2
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%description
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. The goal is to provide a common interface to
classical game problems (loading graphics eg.), so games can share as much
code as possible. Ideally anyone with small resources should be able to
write commercial quality games.

%description -l pt_BR
A Clanlib é uma biblioteca de jogos multi-plataforma desenhada para facilitar
o trabalho dos desenvolvedores. A idéia principal é fornecer uma interface 
comum para os problemas clássicos dos jogos (carregar gráficos por exemplo).

%description -l es
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. The goal is to provide a common interface to
classical game problems (loading graphics eg.), so games can share as much
code as possible. Ideally anyone with small resources should be able to
write commercial quality games.

%package devel
Summary: ClanLib include files for developers
Summary(pt_BR): Arquivos para desenvolvimento usando a Clanlib
Summary(es):  ClanLib include files for developers
Group: Development
Group(pt_BR): Desenvolvimento
Group(es): Desarrollo
PreReq: %{name} = %{version}-%{release}

%description devel
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. The goal is to provide a common interface to
classical game problems (loading graphics eg.), so games can share as much
code as possible. Ideally anyone with small resources should be able to
write commercial quality games.

%description -l pt_BR devel
Arquivos que possibilitam o desenvolvimento de aplicativos utilizando a
biblioteca Clanlib.

%description -l es devel
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. The goal is to provide a common interface to
classical game problems (loading graphics eg.), so games can share as much
code as possible. Ideally anyone with small resources should be able to
write commercial quality games.

%package doc
Summary: ClanLib documentation
Summary(pt_BR): Documentação da ClanLib
Summary(es): ClanLib documentation
Group: Documentation
Group(pt_BR): Documentação
Group(es): Documentación
Conflicts: ClanLib-docs < 0.5.0-10cl
Obsoletes: ClanLib-docs < 0.5.0-10cl

%description doc
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. The goal is to provide a common interface to
classical game problems (loading graphics eg.), so games can share as much
code as possible. Ideally anyone with small resources should be able to
write commercial quality games.

This part contains the documentation. At the moment this includes:
Unix manual page reference documentation.
HTML format reference documentation.

%description -l pt_BR doc
Documentação sobre a biblioteca Clanlib, possui:
 - Referências em formato de páginas de manual (man page) e formato HTML

%description -l es doc
The ClanLib Game SDK is a crossplatform game library designed to ease the
work for game developers. The goal is to provide a common interface to
classical game problems (loading graphics eg.), so games can share as much
code as possible. Ideally anyone with small resources should be able to
write commercial quality games.

This part contains the documentation. At the moment this includes:
Unix manual page reference documentation.
HTML format reference documentation.

%prep
%setup -q

%build
%ifarch i386 i485 i586 i686
CXXFLAGS="-O6 -mpentium -malign-loops=2 -malign-functions=2 -malign-jumps=2 -fomit-frame-pointer"
CFLAGS="-O6 -mpentium -malign-loops=2 -malign-functions=2 -malign-jumps=2 -fomit-frame-pointer"
%endif
autoconf
%configure --enable-dyn --disable-ptc --enable-x11 --enable-fbdev --enable-svgalib --enable-clansound --enable-network --enable-opengl --enable-mikmod --enable-magick --enable-png

make
make clanCore
make clanGL
make clanMikMod
make clanMagick
make clanGUI
#make clanLua
#make clanMPEG
make clanPNG
#make clanVorbis
make strip
make docs

%install
rm -rf %{buildroot}
(
cd Documentation/Reference
mv Makefile Makefile.old
cat Makefile.old | sed -e "
s@^HTML_PREFIX = .*@HTML_PREFIX = %{_builddir}/%{name}-%{version}/doc2inst@g;\
s@^MAN_PREFIX = .*@MAN_PREFIX = %{buildroot}%{_mandir}@g" > Makefile
)

if [ -d doc2inst ]; then
rm -rf doc2inst
fi
mkdir doc2inst

make 	prefix=%{buildroot}%{_prefix} \
	mandir=%{buildroot}%{_mandir} \
	INC_PREFIX=%{buildroot}%{_includedir} \
	BIN_PREFIX=%{buildroot}%{_bindir} \
	LIB_PREFIX=%{buildroot}%{_libdir} \
	TARGET_PREFIX=%{buildroot}%{_libdir}/%{name} \
	install docs_install
#make prefix=%{buildroot}%{_prefix} mandir=%{buildroot}/%{_mandir} docs_install

%post
ldconfig -n %{_libdir}

%postun
ldconfig -n %{_libdir}

%clean
rm -rf %{buildroot}

%files
%defattr(0644,root,root,0755)
%{_libdir}/lib*.so.*
%dir %{_libdir}/%{name}
%{_libdir}/%{name}/*
%doc README COPYING README.fbdev README.upgrade

%files devel
%defattr(0644,root,root,0755)
%doc CREDITS BUGS ascii-logo INSTALL.linux
%{_libdir}/*.so
%{_includedir}/%{name}
%attr(0755,root,root) %{_bindir}/*

%files doc
%defattr(0644,root,root,0755)
%doc doc2inst/*
%{_mandir}/man1/*
%{_mandir}/man3/*

%changelog
* Sun Mar 25 2001 Arnaldo Carvalho de Melo <acme@conectiva.com>
+ ClanLib-0.5.0-10cl
- recompiled with glibc 2.2 (atexit ->  __cxa_atexit)
- include libbz2-devel and Hermes-devel in the BuildRequires list
- devel PreReqs main, not just Requires
- ldconfig -n

* Mon Feb 19 2001 Rodrigo Barbosa <rodrigob@conectiva.com>
+ ClanLib-0.5.0-9cl
- Fixing the build of the extra graphic targets
- Fixing install process

* Wed Feb 14 2001 Rodrigo Barbosa <rodrigob@conectiva.com>
+ ClanLib-0.5.0-8cl
- Building extra graphic targets

* Wed Feb 14 2001 Rodrigo Barbosa <rodrigob@conectiva.com>
+ ClanLib-0.5.0-7cl
- Fixed file listing
- Fixed BuildRequires and Requires
- Needed to bugid #1022

* Tue Dec 05 2000 Magic Bumper <dist@conectiva.com.br>
- Automatically increased release

* Thu Oct 19 2000 Arnaldo Carvalho de Melo <acme@conectiva.com.br>
- one library was listed on both ClanLib and ClanLib-devel, dot patch :)

* Thu Oct 19 2000 Arnaldo Carvalho de Melo <acme@conectiva.com.br>
- /usr/include/ClanLib and /usr/lib/ClanLib _are_ part of these packages
- some cleanups in the %%files sections

* Sat Oct 14 2000 Magic Bumper <dist@conectiva.com.br>
- Automatically increased release

* Thu Oct 12 2000 Rodrigo Barbosa <rodrigob@conectiva.com>
- Treating the man pages and docs the way they should be

* Wed Oct 11 2000 Rodrigo Barbosa <rodrigob@conectiva.com>
- Enabled clanMagick extension (needed by pingus)
- Update to 0.5.0

* Sat Sep 23 2000 Guilherme Wunsch Manika <gwm@conectiva.com>
- updated to 0.4.4
- fixed man and doc dirs

* Wed Jun 21 2000 Flávio Bruno Leitner <flavio@conectiva.com>
- Fixed doc dir permission

* Thu May 04 2000 Elvis Pfützenreuter <epx@conectiva.com>
- Fixed %%doc permissions

* Tue Apr 04 2000 Rudá Moura <ruda@conectiva.com>
- %%ifarch intel

* Sat Jan 08 2000 Guilherme Wunsch Manika <gwm@conectiva.com>
- Updated to 0.2.3

* Sat Jan 08 2000 Flávio Bruno Leitner <flavio@conectiva.com>
- Adopted.
