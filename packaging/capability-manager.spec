Name:		capability-manager
Summary:	Watch Capability Manager
Version:	0.0.7
Release:	1
Group:		connectivity
License:	Apache License, Version 2.0
Source:		%{name}-%{version}.tar.gz

Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(bundle)
BuildRequires:  pkgconfig(libxml-2.0)
BuildRequires:  pkgconfig(capi-appfw-package-manager)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(pkgmgr)
BuildRequires:  pkgconfig(pkgmgr-info)

#Provides:	libcapability-manager.so

%description
Watch Capability Manager

%package devel
Summary:    	Watch Capability Manager (devel)
Group:		Development/Libraries
Requires:   	%{name} = %{version}-%{release}
%description devel
Watch Capability Manager (devel)

%prep
%setup -q

%build

%if 0%{?tizen_build_binary_release_type_eng}
export CFLAGS="$CFLAGS -DTIZEN_ENGINEER_MODE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_ENGINEER_MODE"
export FFLAGS="$FFLAGS -DTIZEN_ENGINEER_MODE"
%endif

cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}/usr/share/license
cp LICENSE %{buildroot}/usr/share/license/%{name}

%post

%postun

%files
%manifest capability-manager.manifest
%defattr(-,root,root,-)
%{_libdir}/libcapability-manager.so*
/usr/share/license/%{name}

%files devel
%defattr(-,root,root,-)
%{_includedir}/*.h
%{_libdir}/pkgconfig/capability-manager.pc

