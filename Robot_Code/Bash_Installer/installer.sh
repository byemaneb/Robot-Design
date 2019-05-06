#!/bin/bash

echo ""
echo "My Command Line Installer"
echo ""

# Create destination folder
DESTINATION="Robot_Control"
mkdir -p ${DESTINATION}

# Find __ARCHIVE__ maker, read archive content and decompress it
ARCHIVE=$(awk '/^__ARCHIVE__/ {print NR + 1; exit 0; }' "${0}")
tail -n+${ARCHIVE} "${0}" | tar xpJv -C ${DESTINATION}

# Put your logic here (if you need)

echo ""
echo "Installation complete."
echo ""

mv ./Robot_Control/files/uninstaller.sh ./

python3 ./Robot_Control/files/udp_send.py
# Exit from the script with success (0)
exit 0

__ARCHIVE__
�7zXZ  �ִF !   t/��'�] 3Iֻ�Rw�����,'���8�� 
8˥:����0=��f���?��K=ܢ�W���p�!jLQ*�������=�Ò�%y�]��LC����h�y�SFX�b��O�䏊]p��+"lTN+�o�JyU�X@X���)�/ ,%0HG��䤉L���x��Tq�_�a)n�ǂ��XeA}1'��T$z�b�z���(6�`�{1���1���ƩyܱLL�w��A hX�X6hQ�JKᘭeO���]��� ������і��l�n��y�%h�OZ��a��u"��CMS"O3t����j���1%�&�_�%�|Q�*�z7bn>�9\Ν4��Ѷ��۶E�T�?�}b�d���ì��`���j�X����/�����-B�`�]�����G��~��)ŸDnE�h���9�Ѫ![/upi��7j��¹_�_>+�����fyw��TD�`E^b΂ڊ�c���}��������X��f�z����8�H���}ŀ��Y��Sp�K�$�7�3�i,+�ne�?-��b����t�F"�|�����E��;}i�6��5�f�;3`��!WѺ+�t��Y��cjξ��������q=�������0�
o*���,1���"8C��bs|�q��/ZQ?�GPT������t^�W����*>�ʕ~?��
��b<n�T�@���l�ތΝ���+�~���$t�������F.�{��K���*񡟆�+ ���~�P��z��y�H�Dk�DE��%{V�����܃����}�X�O�Rd�K�c$6�B�.P�F��23�8�1�"��ǌ���5�oD5&�e��$�}��٢���E$�-mVaB*''�yF_
�
YQ��M�q���.�G��d����"�*k�;0ܳ��W�i�B��͎���\E��ث�G+�m�' ��O��?ㅶ�X=w��*S'�S�y�D����߭�ꉱ>�3���J]Uo}!�����В�Z"�Q�&4c��,\�    ;q�K�^� ��P  
8ӱ�g�    YZ