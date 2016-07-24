
#include "sqrl_internal.h"
#include "SqrlBase64.h"

std::string *SqrlBase64::encode( std::string *dest, const std::string *src, bool append ) {
	static const char B64_ENC_TABLE[64] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
		'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
		'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
		'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y', 'z', '0', '1', '2', '3',
		'4', '5', '6', '7', '8', '9', '-', '_'
	};
	size_t src_len = src->length();
	if( !dest ) dest = new std::string();
	if( append ) {
		dest->reserve( dest->length() + (size_t)(src_len * 4.0 / 3.0) );
	} else {
		dest->clear();
		dest->reserve( (size_t)(src_len * 4.0 / 3.0) );
	}
	size_t i = 0;
	uint32_t tmp;
	char str[4];
	std::string::const_iterator it = src->cbegin();
	while( it != src->cend() ) {
		tmp = (*it++ & 0xFF) << 16;
		if( it != src->cend() ) tmp |= (*it++ & 0xFF) << 8;
		if( it != src->cend() ) tmp |= (*it++ & 0xFF);

		str[0] = B64_ENC_TABLE[(tmp >> 18) & 0x3F];
		str[1] = B64_ENC_TABLE[(tmp >> 12) & 0x3F];
		str[2] = B64_ENC_TABLE[(tmp >> 6) & 0x3F];
		str[3] = B64_ENC_TABLE[tmp & 0x3F];
		dest->append( str, 4 );
	}
	i = (src_len % 3);
	if( i ) i = 3 - i;
	while( i && dest->length() ) {
		dest->pop_back();
		i--;
	}
	return dest;
}

std::string *SqrlBase64::decode( std::string *dest, const std::string *src, bool append ) {
	size_t input_length = src->length();
	if( !dest ) dest = new std::string();
	if( append ) {
		dest->reserve( dest->length() + (size_t)(input_length *3.0 / 4.0) );
	} else {
		dest->clear();
		dest->reserve( (size_t)(input_length * 3.0 / 4.0) );
	}
	size_t i = 0;
	int charCount = 0;
	uint32_t tmp = 0, val;
	char str[4] = {0};

	std::string::const_iterator it = src->cbegin();

	while( it < src->cend() ) {
		if( this->nextValue( &val, it, src->cend() ) ) {
			tmp = val << 18;
			charCount++;
		}
		if( this->nextValue( &val, it, src->cend() ) ) {
			tmp |= val << 12;
			charCount++;
		}
		if( this->nextValue( &val, it, src->cend() ) ) {
			tmp |= val << 6;
			charCount++;
		}
		if( this->nextValue( &val, it, src->cend() ) ) {
			tmp |= val;
			charCount++;
		}

		str[0] = (char)((tmp >> 16) & 0xFF);
		str[1] = (char)((tmp >> 8) & 0xFF);
		str[2] = (char)(tmp & 0xFF);
		dest->append( str, 3 );
	}
	charCount = (charCount % 4);
	if( charCount ) charCount = 4 - charCount;
	while( charCount && dest->length() ) {
		dest->pop_back();
		charCount--;
	}
	return dest;
}

bool SqrlBase64::nextValue( uint32_t *nextValue, std::string::const_iterator &it, std::string::const_iterator &end ) {
	static const char B64_DEC_TABLE[256] = {
		/*   0 */	'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
		/*  16 */	'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
		/*  32 */	'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x3e', '\x00', '\x00',
		/*  48 */	'\x34', '\x35', '\x36', '\x37', '\x38', '\x39', '\x3a', '\x3b', '\x3c', '\x3d', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
		/*  64 */	'\x00', '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0a', '\x0b', '\x0c', '\x0d', '\x0e',
		/*  80 */	'\x0f', '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x00', '\x00', '\x00', '\x00', '\x3f',
		/*  96 */	'\x00', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f', '\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27', '\x28',
		/* 112 */	'\x29', '\x2a', '\x2b', '\x2c', '\x2d', '\x2e', '\x2f', '\x30', '\x31', '\x32', '\x33', '\x00', '\x00', '\x00', '\x00', '\x00',
		/* 128 */	'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
		/* 144 */	'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
		/* 160 */	'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
		/* 176 */	'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
		/* 192 */	'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
		/* 208 */	'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
		/* 224 */	'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
		/* 240 */	'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
	};
	while( it < end ) {
		if( (it)[0] == 'A' ) { // Legitimately return 0x00
			*nextValue = B64_DEC_TABLE[(int)(it[0])];
			++it;
			return true;
		}
		if( B64_DEC_TABLE[(uint8_t)(it[0])] != 0 ) { // Legitimate character
			*nextValue = B64_DEC_TABLE[(int)(it[0])];
			++it;
			return true;
		}
		++it;
	}
	*nextValue = 0;
	return false;
}