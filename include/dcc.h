/*
 * dcc.h: Things dealing client to client connections. 
 *
 * Copyright(c) 1998 Colten Edwards 
 *
 */
#ifndef DCC_H_
#define DCC_H_

#include "struct.h"

extern DCC_dllcommands *dcc_dllcommands;

	/* 
	 * these are all used in the bot_link layer. the dcc_printf is used in
	 * a few other places as well. ie dcc.c
	 */
	int	BX_dcc_printf (int, char *, ...);
	void	tandout_but (int, char *, ...);
	void	chanout_but (int, char *, ...);
	int	handle_tcl_chan (int, char *, char *, char *);
	int	tand_chan (int, char *);
	int	tand_zapf (int, char *);
	int	tand_zapfbroad (int, char *);
	int	handle_dcc_bot (int, char *);
	int	tandem_join (int, char *);
	int	tandem_part (int, char *);
	int	send_who_to (int, char *, int);
	int	tand_who (int, char *);
	int	tand_whom (int, char *);
	int	tell_who (int, char *);
	int	send_who (int, char *);
	int	tell_whom (int, char *);
	int	send_whom (int, char *);
	int	tand_priv (int, char *);
	int	tand_boot (int, char *);
	int	tand_privmsg (int, char *);
	int	tand_part (int, char *);
	int	tand_join (int, char *);
	int	tand_clink (int, char *);
	int	tand_command (int, char *);
	int	cmd_cmsg (int, char *);
	int	cmd_cboot (int, char *);
	int	cmd_act (int, char *);
	int	cmd_help (int, char *);
	int	cmd_msg (int, char *);
	int	cmd_say (int, char *);
	int	cmd_tcl (int, char *);
	int	cmd_chat (int, char *);
	int	cmd_quit (int, char *);
	int	cmd_invite (int, char *);
	int	cmd_echo (int, char *);
	int	cmd_boot (int, char *);
	int	cmd_ops (int, char *);
	int	cmd_adduser (int, char *);
	int	cmd_ircii (int, char *);
	int	cmd_whoami (int, char *);
	int	send_command (int, char *);
	
	int	dcc_ftpcommand (char *, char *);
	

/* 
 * these definitions are mostly used by ircII as well 
 * I expanded the flags to a full 32 bits to allow for future
 * expansion. 
 */
#define DCC_PACKETID  0xfeab		/* used to figure out endianess 
					 * as well as identify the resend
					 * packet 
					 */
#define MAX_DCC_BLOCK_SIZE 16384	/* 
					 * this is really arbritrary value.
					 * we can actually make this a lot
					 * larger and things will still work
					 * as expected. The network layer places
					 * a limit however. 
					 */

#define DCC_CHAT	0x00000001

#define DCC_FILEOFFER	0x00000002
#define DCC_FILEREAD	0x00000003

#define	DCC_RAW_LISTEN	0x00000004
#define	DCC_RAW		0x00000005

#define DCC_REFILEOFFER	0x00000006
#define DCC_REFILEREAD	0x00000007

#define DCC_BOTMODE	0x00000008
#define DCC_FTPOPEN	0x00000009
#define DCC_FTPGET	0x0000000a
#define DCC_FTPSEND	0x0000000b
#define DCC_FTPCOMMAND	0x0000000c
#define DCC_TYPES	0x000000ff

#define DCC_WAIT	0x00010000
#define DCC_ACTIVE	0x00020000
#define DCC_OFFER	0x00040000
#define DCC_DELETE	0x00080000
#define DCC_TWOCLIENTS	0x00100000
#define DCC_RESUME_REQ	0x00200000
#define DCC_QUEUE	0x00400000
#define DCC_TDCC	0x00800000
#define DCC_BOTCHAT	0x01000000
#define DCC_ECHO	0x02000000
#define DCC_SSL		0x04000000
#define DCC_STATES	0xffffff00

/* This collects together the various broken-out fields of a
 * CTCP DCC offer, because they're passed around as a group.
 */
struct dcc_offer {
	char *nick;
	char *type;
	char *description;
	char *address;
	char *port;
	char *size;
	char *extra;
	char *userhost;
};

	int	check_dcc_list (char *);
	int	dcc_exempt_save (FILE *);

	void	BX_dcc_filesend(char *, char *);
	void	BX_dcc_resend(char *, char *);
	void	dcc_stats(char *, char *);
	void	dcc_chat(char *, char *);
	void	dcc_ftpopen(char *, char *);
	void	dcc_glist(char *, char *);
	void	dcc_chatbot(char *, char *);
	void	dcc_resume(char *, char *);
	void	dcc_rename(char *, char *);

	int	BX_get_active_count(void);
	int	dcc_ftpcommand(char *, char *);
	void	process_dcc(char *);
	int	dcc_activechat(char *);	/* identify all active chat dcc's */
	int	dcc_activebot(char *);	/* identify all active bot's */
	int	dcc_activeraw(char *);  /* identify all active raw connects */
	void	dcc_chat_transmit(char *, char *, char *, unsigned);
	void	dcc_bot_transmit(char *, char *, unsigned);
	void	dcc_raw_transmit(char *, char *, unsigned);

	void	handle_dcc_offer(struct dcc_offer *);

	void	dcc_reject(char *, char *, char *);
	char	*dcc_raw_connect(char *, unsigned short);
	char	*dcc_raw_listen(int port);
	void	close_all_dcc(void);
	void	dcc_sendfrom_queue(void);
	void	dcc_check_idle(void);
	int	check_dcc_socket(int);
	char	*get_dcc_info(SocketList *, DCC_int *, int);
	void	init_dcc_table(void);
	int BX_remove_all_dcc_binds(const char *);
	int BX_remove_dcc_bind(char *, int);

/* Function pointers for the operations implementing a DCC type.
 * This structure is part of the module ABI - if you change it,
 * you need to roll MODULE_VERSION in <module.h>.
 *
 * .init() is called when a matching DCC offer is received over IRC
 * .open() is called when a DCC connection is established
 * .input() is called to fetch some data when the socket becomes readable
 * .output() is called to write a message to the socket
 * .close() is called when the DCC connection is closed
 */
struct dcc_ops
{
	int (*init)(const char *, const char *, const char *, const char *, const char *, const char *, unsigned long, unsigned short);
	int (*open)(int, int, unsigned long, unsigned short);
	int (*input)(int, int, char *, int, int);
	int (*output)(int, int, char *, int);
	int (*close)(int, unsigned long, unsigned short);
};
	
	int	BX_add_dcc_bind(char *, char *, const struct dcc_ops *);
/* add_socketread() callbacks for ordinary CHAT and SEND DCCs */
extern void BX_dcc_chat_socketread(int);
extern void BX_dcc_send_socketread(int);

	SocketList *BX_find_dcc(const char *, const char *, const char *, int, int, int, int);	
	void	BX_erase_dcc_info(int, int, char *, ...);
	DCC_int	*BX_dcc_create(char *, char *, char *, unsigned long, int, int, unsigned long, void (*func)(int));
	int	close_dcc_number(int);
	
	char *	equal_nickname (const char *);
	
#define DCC_STRUCT_TYPE  0xdcc0dcc0

#endif /* DCC_H_ */
