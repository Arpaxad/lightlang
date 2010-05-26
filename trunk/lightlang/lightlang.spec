%define svn_build 1

%if %{svn_build}
%define build_version %(date +%%Y%%m%%d)svn%{?dist}
%else
%define build_version 1
%endif


Name: lightlang
Version: 0.8.6
Release: %{build_version}
Summary: Powerful system of electronic dictionaries for Linux
Group: Applications/Office
License: GPL
URL: http://code.google.com/p/lightlang

Source0: %{name}-%{version}.tar.bz2
BuildRoot: %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)
BuildRequires: PyQt4, python-xlib
Requires: PyQt4, python-xlib, mplayer

%description
%{summary}


%package devel
Summary: Devel package for LightLang
Requires: %{name}, pkgconfig
Group: Applications/Office

%description devel
%{summary}


%prep
%if %{svn_build}
%setup -q -n %{name}
%else
%setup -q
%endif


%build
%configure \
	--with-audio-player=mplayer
make %{?_smp_mflags}


%install
rm -rf ${RPM_BUILD_ROOT}
make DESTDIR=${RPM_BUILD_ROOT} install
mkdir -p ${RPM_BUILD_ROOT}/%{_localstatedir}/lib
mv -v ${RPM_BUILD_ROOT}/%{_datadir}/sl ${RPM_BUILD_ROOT}/%{_localstatedir}/lib/%{name}
ln -s ../../%{_localstatedir}/lib/%{name} ${RPM_BUILD_ROOT}/%{_datadir}/sl


%clean
rm -rf ${RPM_BUILD_ROOT}


%files
%defattr(-,root,root,-)
%{_bindir}/sl
%{_bindir}/xsl
%{_bindir}/llrepo
%{_bindir}/lightlang
%{_libdir}/xsl
%{_libdir}/llrepo
%{_datadir}/applications/xsl.desktop
%{_datadir}/icons/hicolor/256x256/apps/*.png
%{_datadir}/icons/hicolor/128x128/apps/*.png
%{_datadir}/icons/hicolor/64x64/apps/*.png
%{_datadir}/icons/hicolor/48x48/apps/*.png
%{_datadir}/icons/hicolor/32x32/apps/*.png
%{_datadir}/icons/hicolor/22x22/apps/*.png
%{_datadir}/icons/hicolor/16x16/apps/*.png
%{_localstatedir}/lib/%{name}
%{_datadir}/sl
%{_datadir}/xsl
%doc %{_docdir}/lightlang
%doc %{_mandir}/man1/*.gz
%doc %{_mandir}/ru/man1/*.gz


%files devel
%{_libdir}/pkgconfig/lightlang.pc


%changelog
* Wed May 26 2010 Devaev Maxim <mdevaev@gmail.com> 1.0-20100526svn
- Initial build

