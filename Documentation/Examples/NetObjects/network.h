/* $Id: network.h,v 1.4 2001/11/05 11:23:02 sphair Exp $ */

#ifndef _NETWORK_H_
#define _NETWORK_H_

#define NETWORK_PORT	0xDEAD
#define NETWORK_ID		"Brutal2"

// Define which netchannels are used in various classes.
// It only defines the first netchannel, and the class use this as a base-value.
#define NETCHANNELS_LOBBY	1000
#define NETCHANNELS_WORLD	2000
#define NETCHANNELS_PLAYERS	2500

// CTS = Client To Server
// STC = Server To Client

#endif