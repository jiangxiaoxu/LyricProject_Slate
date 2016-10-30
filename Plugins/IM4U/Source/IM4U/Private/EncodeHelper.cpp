
#include "IM4UPrivatePCH.h"
#include "EncodeHelper.h"


uint32 EncodeHelper::string_to_codepage(std::string code_str)
{
#ifdef _WIN32
	std::transform(code_str.begin(), code_str.end(), code_str.begin(), ::tolower);

	if (code_str == "shift_jis" || code_str == "shift-jis") {
		return CP_ACP;
	}

	if (code_str == "utf-8") {
		return CP_UTF8;
	}
#endif
	// 
	return 0;
}

#ifdef _WIN32
std::string EncodeHelper::convert_encoding(const std::string &str, const char *fromcode, const char *tocode)
{
	uint32 from_code = string_to_codepage(std::string(fromcode));
	uint32 to_code = string_to_codepage(std::string(tocode));

	WCHAR *utf16str;
	char *outstr;
	int rtn_len;

	// utf-16Ç…ïœä∑
	int utf16str_len = MultiByteToWideChar(from_code, 0, str.c_str(), -1, NULL, 0);
	utf16str = new WCHAR[utf16str_len + 1];
	rtn_len = MultiByteToWideChar(from_code, 0, str.c_str(), -1, utf16str, utf16str_len);
	utf16str[rtn_len] = L'\0';

	// utf-16Ç©ÇÁîCà”ÇÃï∂éöÉRÅ[ÉhÇ…ïœä∑
	int outstr_len = WideCharToMultiByte(to_code, 0, utf16str, -1, NULL, 0, NULL, NULL);
	outstr = new char[outstr_len + 1];
	rtn_len = WideCharToMultiByte(to_code, 0, utf16str, -1, outstr, outstr_len, NULL, NULL);
	outstr[rtn_len] = '\0';

	std::string s(outstr);
	delete[] utf16str;
	delete[] outstr;

	return s;
}
#else
std::string EncodeHelper::convert_encoding(const std::string &str, const char *fromcode, const char *tocode)
{
	char *outstr, *instr;
	iconv_t icd;
	size_t instr_len = std::strlen(str.c_str());
	size_t outstr_len = instr_len * 2;

	if (instr_len <= 0) return "";

	// allocate memory
	instr = new char[instr_len + 1];
	outstr = new char[outstr_len + 1];
	strcpy(instr, str.c_str());
	icd = iconv_open(tocode, fromcode);
	if (icd == (iconv_t)-1) {
		return "Failed to open iconv (" + std::string(fromcode) + " to " + std::string(tocode) + ")";
	}
	char *src_pos = instr, *dst_pos = outstr;
	if (iconv(icd, &src_pos, &instr_len, &dst_pos, &outstr_len) == -1) {
		// return error message
		std::string errstr;
		int err = errno;
		if (err == E2BIG) {
			errstr = "There is not sufficient room at *outbuf";
		}
		else if (err == EILSEQ) {
			errstr = "An invalid multibyte sequence has been encountered in the input";
		}
		else if (err = EINVAL) {
			errstr = "An incomplete multibyte sequence has been encountered in the input";
		}
		iconv_close(icd);
		return "Failed to convert string (" + errstr + ")";
	}
	*dst_pos = '\0';
	iconv_close(icd);

	std::string s(outstr);
	delete[] instr;
	delete[] outstr;

	return s;
}
#endif
