
                     MagicCrypto ��ȣ��� ��뼳��

                           (��)�帲��ť��Ƽ

===========================================================================
      �� ������ MagicCrypto ��ȣ���� Openssl �� Openssl�� ����ϴ� 
      �������α׷��� �����Ͽ� ����ϱ� ���� ������ �����մϴ�. 
===========================================================================

1. �Ұ�

MagicCrypto ��ȣ����� KS X ISO/IEC 24759 ��ȣ��� ����䱸������ �����ϸ�
������������ ������ ���� ��ȣ����Դϴ�. ���� MagicCrypto ��ȣ����� Windo-
ws, Linux, Unix �� �پ��� �÷������� �����մϴ�. 


2. MagicCrytpo ��ȣ��� ��ġ

MagicCrypto ��ȣ����� ����������̺귯��(�Ǵ� �������̺귯��)�̸� ��������
�� ������ �����ϴ�.

* �⺻����

	libMagicCrypto.[ext]
	- [ext]�� �� �÷������� �����ϴ� �������̺귯�� Ȯ�����Դϴ�. 
	  Ȯ���� : Windows (dll), Linux (so), SunOS (so)
		   HP-UX (sl), IBM AIX (a), MacOS (dylib)

	- �� ���������� Linux�� �������� �����մϴ�.


* ��������

	��ȣ���
		libMagicCrypto.so
		mcapi.h
		mcapy_type.h
	�׽���
		tester.c



* ��ġ

	��ȣ����� ����ϱ����� ������ �����Ͽ� �ش������� ��ȣ����� ����
	�մϴ�.


* ȯ�漳��

	��ȣ����� ��ġ�� ������ �� �÷������� �����ϴ� �������̺귯�� ȯ��
	������ �����մϴ�. 

	- Windows (PATH), Linux (LD_LIBRARY_PATH), SunOS (LD_LIBRARY_PATH)
	  HP-UX (SHLIB_PATH), IBM AIX (LIBPATH), MacOS (DYLD_LIBRARY_PATH)


* �׽��� ������ �ɼ� �߰� 
	-ldl : 'dladdr' ���� ��
	-lrt : 'clock_gettime' ���� ��

