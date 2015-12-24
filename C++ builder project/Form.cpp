//---------------------------------------------------------------------------

#include <vcl.h>
#include  <iostream>
#include  <fstream>
#include <sys\stat.h>
#pragma hdrstop
#include "Form.h"
#include "CryptONPCH1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cgauges"
#pragma resource "*.dfm"
TMForm *MForm;
//---------------------------------------------------------------------------
__fastcall TMForm::TMForm(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------

void __fastcall TMForm::b_helpClick(TObject *Sender)
{
ShellExecuteA(NULL, "open", "http://sites.google.com/site/cryptoninf/home/info", NULL, NULL, SW_SHOW);
}
//---------------------------------------------------------------------------

void __fastcall TMForm::b_openClick(TObject *Sender)
{
	if (OpenD->Execute()) {
filen = OpenD->FileName;
int file_name_l = filen.Length();//���������� ����� ����� �����(+����)
//char *ex = new char[4];//������ ���������� �����
char ex[5];//������ ���������� �����
int k = 0;//������ ����� ���������� ����������� ex
for (int i = file_name_l - 3; i <= file_name_l; i++) {
ex[k] = filen[i];
k++;
}
l_pass->Enabled=true;
fpass->Enabled=true;
password->Enabled=true;
//��������� ����������� ����������� ��� ������������� ����
if((ex[0] == 'c'&&ex[1] == 'r'&&ex[2] == 'p'&&ex[3] == 't') || (ex[0] == 'C'&&ex[1] == 'R'&&ex[2] == 'P'&&ex[3] == 'T')) { //���� ������ ������������� ����
//if(ex=="CRPT") {
b_lock->Enabled = false; //������ ����������� ����������
b_unlock->Enabled = true; //�������� ������ ������������
}
else{ //���� ������ ������ ����
b_unlock->Enabled = false; //������ ������������ ����������
b_lock->Enabled = true; //������ ����������� ��������
}
}
}
//---------------------------------------------------------------------------

void __fastcall TMForm::b_lockClick(TObject *Sender)
{
if (password->Text=="")
ShowMessage("���� ����� �� ����� ���� ������!");
else{
AnsiString filenew=filen+".crpt";
DeleteFile(filenew);
fCoDec(filen,filenew, true);
l_pass->Enabled=false;
fpass->Clear();
fpass->Enabled=false;
password->Clear();
password->Enabled=false;
b_lock->Enabled=false;
y->Visible=false;
n->Visible=false;
}
}
//---------------------------------------------------------------------------

void __fastcall TMForm::b_unlockClick(TObject *Sender)
{
if (password->Text=="")
	ShowMessage("���� ����� �� ����� ���� ������!");
else{
AnsiString filenew=filen;
int index = filenew.Pos(".crpt");
if( index != 0 ) {
   filenew.Delete(index,5);	   //������� ���������� ������������ �����
}
DeleteFile(filenew);
fCoDec(filen,filenew, false);
l_pass->Enabled=false;
fpass->Clear();
fpass->Enabled=false;
password->Clear();
password->Enabled=false;
b_unlock->Enabled=false;
y->Visible=false;
n->Visible=false;
}
}
//---------------------------------------------------------------------------
 void TMForm::fCoDec(AnsiString Pt1, AnsiString Pt2, bool enco)
{
	if(cmppass()){
	const cmax = 524288000;//  ������������ ������ ����� - 500 ��
	AnsiString pass = password->Text;
    HCRYPTPROV  hProv ;              //  ���������
	HCRYPTHASH  hash ;               //  ���
    HCRYPTKEY   key ;                //  ����
	UCHAR *bufC;                     //  ����� ��� ���������
	DWORD len;                       //  ����� ������ � �����
//    �������� �������� ����������������
    CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
//    ������� ���-������
    CryptCreateHash(hProv, CALG_SHA, 0, 0, &hash);
//    �������� ������
	CryptHashData(hash, pass.c_str(), pass.Length(), 0);
//    ������� ���� �� ��������� ������ ��� ���������� ����� RC2
	CryptDeriveKey(hProv, CALG_RC2, hash, 0, &key);
//    ���������� ���-������
    CryptDestroyHash(hash);
//    ��������� �����
	 ifstream  fin(Pt1.c_str(),ios::in|ios::binary) ;
	 ofstream fout(Pt2.c_str(),ios::out|ios::binary) ;
//    �������� ����� ��� ������
    bufC = new UCHAR[cmax] ;  //  �������� ������
    fin.read(bufC,cmax) ;
	len=fin.gcount() ;
	if (enco){    //    ������� ������
	//clock_t start = clock();
	   CryptEncrypt(key, 0, true, 0, bufC, &len, cmax);
	   ShowMessage("���� "+Pt1+" ������� ����������! ������������� ���� ����� ���������� .crpt");
	  // ShowMessage((clock() - start) / CLOCKS_PER_SEC);
	   }
	else{       //   �������������� ������
	   CryptDecrypt(key, 0, true, 0, bufC, &len );
	   ShowMessage("���� "+Pt2+" ������� ����������!");
	}
	fout.write(bufC,len);
//    ����������� �������� ����������������
    CryptReleaseContext(hProv, 0);
    delete[] bufC;
    fin.close();
	fout.close();
	}
	}
//---------------------------------------------------------------------------
	int TMForm::cmppass(){
	if (fpass->Text==password->Text)
	return 1;
	else{
		ShowMessage("��������� ����� �� ���������!");
		password->SetFocus();
		return 0;
	}
	}
void __fastcall TMForm::passwordChange(TObject *Sender)
{
if(fpass->Text!=password->Text){
	if(n->Visible==false){
		y->Visible=false;
		n->Visible=true;
	}
}else{
	if(y->Visible==false){
		n->Visible=false;
		y->Visible=true;
	}
}
}
//---------------------------------------------------------------------------


