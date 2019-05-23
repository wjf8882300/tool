/***************************************************************
 * SQLFormat.h
 * ϵͳ��  : ���ù��߹���
 * ��ϵͳ��: ���ù�����
 * ��������      ����           ժҪ
 * ----------    -----------    --------------------------------
 * 2011/04/01    ������			���ο���
 ***************************************************************
 */
#ifndef SQLFORMAT_H_
#define SQLFORMAT_H_
#include "util/util.h"
using std::vector;

/**
 * ������
 * @author  ������
 * @version V2.0.00
 * @since V2.0.00
 */
class SQLFormat {
public:	
	/**
	 * ���캯��
	 * @param sql		����ʽ��SQL
	 * @param isQt		�Ƿ��������
	 */
	SQLFormat(CString &sql,bool isQt);
	/**
	 * ��ʽ��SQL���
	 * @strOut		��ʽ�ַ���
	 */
	void parseSql(CString& strOut);
	/**
	 * ��ʽ��SQL���
	 * @strOut		��ʽ�ַ���
	 */
	bool formatSql(CString& strOut);
private:
	/**
	 * ��ʼ���ؼ���
	 */
	void Initlize();
	/**
	 * ��ʽ���ؼ���
	 * @param vcstr		�ؼ��ּ���
	 * @param isBefore	��ʽ��λ���Ƿ���ǰ���뻻�з�
	 */
	void formatKey(const vector<CString>& vcstr,bool isBefore);

	/**
	 * ��ʽ������
	 */
	void judgeQuotes();

	/**
	 * ��ʽ������
	 */
	void formatLength(CString& strOut);

	/**
	 * tab�滻Ϊ�ո��
	 */
	void formatTab();

	/**
	 * �滻����
	 */
	void formatComma();

	/**
	 * ����
	 */
	void formatIndentation();

	/**
	 * ��ʽ���ո�
	 */
	void formatSpace();

	/**
	 * ��ʽ������
	 */
	bool formatColumn(CString& strOut);

	/**
	 * ��ȡ��
	 */
	int findColumn(CString& strOut, const CString& strInput);

	bool judgeColumn(const CString& strInput, CString& strOutput);
private:
	//ǰ�ùؼ���
	vector<CString> vbefore;
	//���ùؼ���
	vector<CString> vbehind;
	//����
	vector<CString> vfunction;

	//����ʽ����
	CString strSql;
	//�Ƿ��˫����
	bool isQuotes;
	//���з�
	CString sepChangeLine;
	//�ո��
	CString sepSpace	 ;
	//����
	CString sepQuotes    ;
	//tab��
	CString sepTab		 ;
};


#endif /*SQLFormat_H_*/