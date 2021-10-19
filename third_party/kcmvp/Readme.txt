
                     MagicCrypto 암호모듈 사용설명서

                           (주)드림시큐리티

===========================================================================
      본 파일은 MagicCrypto 암호모듈과 Openssl 및 Openssl을 사용하는 
      응용프로그램과 연계하여 사용하기 위한 절차를 설명합니다. 
===========================================================================

1. 소개

MagicCrypto 암호모듈은 KS X ISO/IEC 24759 암호모듈 시험요구사항을 만족하며
국가정보원의 검증을 받은 암호모듈입니다. 또한 MagicCrypto 암호모듈은 Windo-
ws, Linux, Unix 등 다양한 플랫폼에서 동작합니다. 


2. MagicCrytpo 암호모듈 설치

MagicCrypto 암호모듈은 동적연결라이브러리(또는 공유라이브러리)이며 구성파일
은 다음과 같습니다.

* 기본사항

	libMagicCrypto.[ext]
	- [ext]는 각 플랫폼에서 지원하는 공유라이브러리 확장자입니다. 
	  확장자 : Windows (dll), Linux (so), SunOS (so)
		   HP-UX (sl), IBM AIX (a), MacOS (dylib)

	- 본 문서에서는 Linux를 기준으로 설명합니다.


* 구성파일

	암호모듈
		libMagicCrypto.so
		mcapi.h
		mcapy_type.h
	테스터
		tester.c



* 설치

	암호모듈을 사용하기위한 폴더를 지정하여 해당폴더에 암호모듈을 복사
	합니다.


* 환경설정

	암호모듈이 설치된 폴더를 각 플랫폼에서 지원하는 공유라이브러리 환경
	변수에 적용합니다. 

	- Windows (PATH), Linux (LD_LIBRARY_PATH), SunOS (LD_LIBRARY_PATH)
	  HP-UX (SHLIB_PATH), IBM AIX (LIBPATH), MacOS (DYLD_LIBRARY_PATH)


* 테스터 컴파일 옵션 추가 
	-ldl : 'dladdr' 오류 시
	-lrt : 'clock_gettime' 오류 시

