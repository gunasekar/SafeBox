/* 
 * File:   steganography.h
 * Author: root
 *
 * Created on January 30, 2011, 8:31 PM
 */

#ifndef _STEGANOGRAPHY_H
#define	_STEGANOGRAPHY_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

char* inscribe(char *InputStream, char *secret, int count);
char* extract(char *InputStream, int count);

#endif	/* _STEGANOGRAPHY_H */

