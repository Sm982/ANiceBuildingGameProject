/**********************************************************************************
    First Growtopia Private Server made with ENet.
    Copyright (C) 2018  Growtopia Noobs

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**********************************************************************************/


#include "stdafx.h"
#include <iostream>
#include <ctime>

#include "enet/enet.h"
#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <cstdio>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif
#ifdef __linux__
#include <stdio.h>
char _getch() {
    return getchar();
}
#endif
#include <vector>
#include <sstream>
#include <chrono>
#include <fstream>
#include "json.hpp"
#ifdef _WIN32
#include "bcrypt.h"
#include "crypt_blowfish/crypt_gensalt.cpp"
#include "crypt_blowfish/crypt_blowfish.h"
#include "crypt_blowfish/crypt_blowfish.cpp"
#include "crypt_blowfish/ow-crypt.cpp"
#include "bcrypt.cpp"
#else
#include "bcrypt.h"
#include "bcrypt.cpp"
#include "crypt_blowfish/crypt_gensalt.h"
#include "crypt_blowfish/crypt_gensalt.cpp"
#include "crypt_blowfish/crypt_blowfish.h"
#include "crypt_blowfish/crypt_blowfish.cpp"
#include "crypt_blowfish/ow-crypt.h"
#include "crypt_blowfish/ow-crypt.cpp"
#include "bcrypt.h"
#endif
#include <thread> // TODO
#include <mutex> // TODO

#pragma warning(disable : 4996)
void  toUpperCase(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}
using namespace std;
using json = nlohmann::json;
// with news banner
//string newslist = "set_default_color|`o\n\nadd_label_with_icon|big|`wThe Growtopia Gazette``|left|5016||\n\nadd_spacer|small|\n\nadd_image_button|banner|interface/large/news_banner.rttex|noflags||||\n\nadd_spacer|small||\n\nadd_textbox|`wApril 2nd:`` `5Updated item data and support for easter.``|left|\n\nadd_spacer|small|\n\n\n\nadd_textbox|Hello Growtopians,|left|\n\nadd_spacer|small|\n\n\n\nadd_textbox|I've started to roll out weather machien system, this feature should be completed within the next week or so. Note there are some bugs with it as well. |\n\nadd_spacer|small|\nadd_label_with_icon|small|`` `#New Feature:`o Weather machine system is now working! Night and Arid machines are the only ones that are work. But weather machines should sync to the worlds, when worlds are automatically saved weather should also update for other players. |left|24||\n\nadd_spacer|small|\nadd_label_with_icon|small|`#New Feature:`o Account Suspension is now a work in progress.|left|24||left||\n\nadd_spacer|small|\n\n\n\nadd_textbox|`o- `6@Matty\n\nadd_spacer|small|\nadd_label_with_icon|small|`4WARNING:`` `5Worlds (and accounts)`` might be deleted at any time if database issues appear (once per day or week).|left|4|\nadd_label_with_icon|small|`4WARNING:`` `5Accounts`` are in beta, bugs may appear and they will be probably deleted often, because of new account updates, which will cause database incompatibility.|left|4|\nadd_spacer|small|\n\nadd_url_button||``Watch: `1Watch a video about GT Private Server``|NOFLAGS|https://www.youtube.com/watch?v=_3avlDDYBBY|Open link?|0|0|\nadd_url_button||``Channel: `1Watch Growtopia Noobs' channel``|NOFLAGS|https://www.youtube.com/channel/UCLXtuoBlrXFDRtFU8vPy35g|Open link?|0|0|\nadd_quick_exit|\n\nend_dialog|gazette|Close||";
// without news banner
//string newslist = "set_default_color|`o\n\nadd_label_with_icon|big|`wThe Growtopia Gazette``|left|5016||\n\nadd_spacer|small|\n\nadd_textbox|`wMarch 23:`` `5Some exciting moderator features!``|left|\n\nadd_spacer|small|\n\n\n\nadd_textbox|Hello Growtopians,|left|\n\nadd_spacer|small|\n\n\n\nadd_textbox|I've added some cool moderator features, some punishing and some fun to mess around with. Also there has been some backend things have been changed. |\n\nadd_spacer|small|\nadd_label_with_icon|small|`` `#New Feature:`o World nuking is now available. Note When a world is nuked, it will stay nuked until the server shuts down.|left|24||\n\nadd_spacer|small|\nadd_label_with_icon|small|`#New Feature:`o Moderators can also warn users for spamming. If they find the person spamming too much, they can also ducttape.|left|24||left|\n\nadd_spacer|small|\nadd_label_with_icon|small|`4WARNING:`` `5Worlds (and accounts)`` might be deleted at any time if database issues appear (once per day or week).|left|4|\nadd_label_with_icon|small|`4WARNING:`` `5Accounts`` are in beta, bugs may appear and they will be probably deleted often, because of new account updates, which will cause database incompatibility.|left|4|\nadd_spacer|small|\n\nadd_url_button||``Watch: `1Watch a video about GT Private Server``|NOFLAGS|https://www.youtube.com/watch?v=_3avlDDYBBY|Open link?|0|0|\nadd_url_button||``Channel: `1Watch Growtopia Noobs' channel``|NOFLAGS|https://www.youtube.com/channel/UCLXtuoBlrXFDRtFU8vPy35g|Open link?|0|0|\nadd_url_button||``Items: `1Item database by Nenkai``|NOFLAGS|https://raw.githubusercontent.com/Nenkai/GrowtopiaItemDatabase/master/GrowtopiaItemDatabase/CoreData.txt|Open link?|0|0|\nadd_quick_exit|\n\nend_dialog|gazette|Close||";
//string newslist = "set_default_color|`o\n\nadd_label_with_icon|big|`wThe Growtopia Gazette``|left|5016||\n\nadd_spacer|small|\n\nadd_textbox|`wApril 8th:`2 ROLLBACK 3! `oOur Hardware upgrade went badly and our data center actually ended up frying our RAID arrays. Sadly we had to restore to a 24 hour backup, so everyone must play that time again. We are truly sorry for the inconvenience!|\n\nadd_textbox|`oTo make it up to you, heres what we did: |\n\nadd_spacer|small|\nadd_label_with_icon|small|`52X: `oAll purchases and Tapjoy offers during the 24 hours we lost have been restored with`w DOUBLE`o the gems.|left|24||\n\nadd_spacer|small|\nadd_label_with_icon|small|`5PLAQUE 3:`o The Rollback 3 Plaque can only be created during the next 24 hours. (Hint: Cave Background + Sign)|left|24||\nadd_label_with_icon|small|`5LUCKY: `oAll players will have the Lucky Mod applied for the next 24 hours or so!|left|24||\nadd_label_with_icon|small|`525% MORE: `oAll IAP gives 25% more gems for the next 24 hours or so|left|24||left|\n\nadd_spacer|small||\nadd_textbox|`o- `6@Matty`o/`#@YoMattty||\n\nadd_spacer|small|\nadd_label_with_icon|small|`4WARNING:`` `5Worlds (and accounts)`` might be deleted at any time if database issues appear (once per day or week).|left|4|\nadd_label_with_icon|small|`4WARNING:`` `5Accounts`` are in beta, bugs may appear and they will be probably deleted often, because of new account updates, which will cause database incompatibility.|left|4|\nadd_spacer|small|\nadd_url_button||``Items: `1Item database by Nenkai``|NOFLAGS|https://raw.githubusercontent.com/Nenkai/GrowtopiaItemDatabase/master/GrowtopiaItemDatabase/CoreData.txt|Open link?|0|0|\nadd_spacer|small|\nadd_url_button||`wWOTD: `1HAPPYBLARNEY`` by `#CREONE````|NOFLAGS|OPENWORLD|HAPPYBLARNEY|0|0|\nadd_spacer|small|\nadd_quick_exit|\n\nend_dialog|gazette|Close||";
//string newslist = "set_default_color|`o\n\nadd_label_with_icon|bib|`wThe Growtopia Gazette|left|5016|\nadd_spacer|small|\nadd_image_button|banner|interface/large/news_banner.rttex|bannerlayout|||\nadd_spacer|small|\nadd_textbox|`wApril 8th: `5Easter Week Ends!``|left|\nadd_spacer|small|\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_feature_btn01.rttex|3imageslayout|OPENSTORE|main/itemomonth|\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_feature_btn02.rttex|3imageslayout|OPENSTORE|main/gems_bundle05|\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_feature_btn03.rttex|3imageslayout|OPENSTORE|main/gems_glory|\nadd_spacer|small|\nadd_textbox|The Easter celebration has come to an end and we are really glad that so many of you have taken part in it!|left|\nadd_spacer|small|\nadd_textbox|For us to know your insights on which topics you want to discuss the most for `2Player Appreciation Week 2021``, we'd like you to fill up this quick survey. Complete the survey on your mobile device to let us know what you think and what you want!|left|\nadd_spacer|small|\nadd_url_button|survey|`wGive us your opinion!``|noflags|OPENSURVEY||\nadd_spacer|small|\nadd_textbox|The new Item of the Month `8Grow X Rocket Wings`` and March's Subscriber Item `8Dashing Dolphin Hat`` chosen by ALEXALORE are out now as well!|left|\nadd_spacer|small|\nadd_textbox|Don't forget to join our `2Growtopia Official Discord Server`` on `2discord.gg/growtopia`` or click the link below!|left|\nadd_spacer|small|\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_community_btn01.rttex|3imageslayout|OPENCOMMUNITY|community_growtorials/TUTORIAL_DOORS|\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_community_btn04.rttex|3imageslayout|https://discord.gg/growtopia|Would you like to join our Discord server?|\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_btn01.rttex|3imageslayout|https://www.youtube.com/watch?v=GaCsg9qp5yw|Would you like to open this in youtube?|\nadd_spacer|small|\nadd_textbox|Visit our Social Media Pages for more content!|left|\nadd_spacer|small|\nadd_image_button|iotm_layout_4|interface/large/gazette/gazette_5columns_social_btn01.rttex|5imageslayout|https://twitter.com/growtopiagame|Would you like to open this in twitter?|\nadd_image_button|iotm_layout_4|interface/large/gazette/gazette_5columns_social_btn02.rttex|5imageslayout|https://www.facebook.com/growtopia|Would you like to open this in facebook?|\nadd_image_button|iotm_layout_4|interface/large/gazette/gazette_5columns_social_btn03.rttex|5imageslayout|https://www.instagram.com/growtopia_official|Would you like to open this in instagram?|\nadd_image_button|iotm_layout_4|interface/large/gazette/gazette_5columns_social_btn04.rttex|5imageslayout|https://www.youtube.com/growtopia_official|Would you like to open this in Youtube?|\nadd_image_button|iotm_layout_4|interface/large/gazette/gazette_5columns_social_btn06.rttex|5imageslayout|https://www.discord.gg/growtopia|Would you like to join our Discord server?|\nadd_spacer|small|\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_policy_btn02.rttex|3imageslayout|https://legal.ubi.com/privacypolicy/en-INTL|Do you want to read the Privacy policy?|\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_policy_btn01.rttex|3imageslayout|https://support.ubisoft.com/en-GB/article/000095632|Do you want to open the Growtopia Code?|\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_policy_btn03.rttex|3imageslayout|https://legal.ubi.com/termsofuse/en-INTL|Do you want to read the Terms of Use?|\nadd_spacer|small|\nadd_spacer|small|\nadd_quick_exit|add_spacer|small|\nadd_url_button|comment|`wVisit Growtopia Forums``|noflags|https://growtopiagame.com/forums|Visit the Growtopia Forums?|0|0|\nadd_spacer|small|\nadd_url_button||`wWOTD: `1None Set`` by `6@Matty````|NOFLAGS|OPENWORLD|BETA|0|0|\nadd_spacer|small|\nadd_url_button||`wVOTW:`1 Fears of the Future (Growtopia Film)``|NOFLAGS|https://www.youtube.com/watch?v=-tUQLn39-Js&t=3|set_survey_enabeled|1|\nend_dialog|gazette||OK|";
//30/04/2021
string newslist = "set_default_color|`o|\nadd_label_with_icon|big|`wThe Growtopia Gazette``|left|5016||\nadd_spacer|small||\nadd_image_button|banner|interface/large/news_banner.rttex|bannerlayout||||\nadd_spacer|small||\nadd_textbox|`wApril 22nd: `5Grow4Good Week Ends!``|left||\nadd_spacer|small||\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_feature_btn01.rttex|3imageslayout|OPENSTORE|main/itemomonth||\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_feature_btn02.rttex|3imageslayout|OPENSTORE|main/gems_bundle05||\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_feature_btn03.rttex|3imageslayout|OPENSTORE|main/gems_glory||\nadd_spacer|small||\nadd_textbox|With `2Grow4Good Week`` ending, we would like to thank everyone for the generosity that you showed both in-game and in real life. We are happy to announce that with your help, we were able to raise a total of `2$10,163.00 for Operation Smile``. With this, they can provide at least `243 new smiles`` to children all over the world.|left||\nadd_spacer|small||\nadd_textbox|We hope that you enjoyed this event, the cool items, and the opportunity to provide surgical care for children with cleft conditions.|left||\nadd_spacer|small||\nadd_textbox|The┬áGrowtopia Survey for `2April 2021 Update``┬áis now open!┬áComplete the survey on your mobile device to let us know what you think!|left||\nadd_spacer|small||\nadd_url_button|survey|`wGive us your opinion!``|noflags|OPENSURVEY|||\nadd_spacer|small||\nadd_textbox|The new Item of the Month `8Grow X Rocket Wings`` and March's Subscriber Item `8Dashing Dolphin Hat`` chosen by ALEXALORE are out now as well!|left||\nadd_spacer|small||\nadd_textbox|Don't forget to join our `2Official Growtopia Discord Server`` on `2discord.gg/growtopia`` or click the link below!|left||\nadd_spacer|small||\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_community_btn01.rttex|3imageslayout|OPENCOMMUNITY|community_growtorials/TUTORIAL_DOORS||\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_community_btn04.rttex|3imageslayout|https://discord.gg/growtopia|Would you like to join our Discord Server?||\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_btn01.rttex|3imageslayout|https://www.youtube.com/watch?v=GaCsg9qp5yw|Would you like to open this in Youtube?||\nadd_spacer|small||\nadd_textbox|Visit our Social Media pages for more Content!|left||\nadd_spacer|small||\nadd_image_button|iotm_layout_4|interface/large/gazette/gazette_5columns_social_btn01.rttex|5imageslayout|https://twitter.com/growtopiagame|Would you like to open this in Twitter?||\nadd_image_button|iotm_layout_4|interface/large/gazette/gazette_5columns_social_btn02.rttex|5imageslayout|https://www.facebook.com/growtopia|Would you like to open this in Facebook?||\nadd_image_button|iotm_layout_4|interface/large/gazette/gazette_5columns_social_btn03.rttex|5imageslayout|https://www.instagram.com/growtopia|Would you like to open this in Instagram?||\nadd_image_button|iotm_layout_4|interface/large/gazette/gazette_5columns_social_btn04.rttex|5imageslayout|https://www.youtube.com/growtopia_official|Would you like to open this in Youtube?||\nadd_image_button|iotm_layout_4|interface/large/gazette/gazette_5columns_social_btn06.rttex|5imageslayout|https://discord.gg/growtopia|Would you like to join our Discord Server?||\nadd_spacer|small||\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_policy_btn02.rttex|3imageslayout|https://legal.ubi.com/privacypolicy/en-INTL|Do you want to read the Privacy Policy?||\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_policy_btn01.rttex|3imageslayout|https://support.ubisoft.com/en-GB/article/000095632|Do you want to read the Growtopian Code?||\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_policy_btn03.rttex|3imageslayout|https://legal.ubi.com/termsofuse/en-INTL|Do you want to read the Terms of Use?||\nadd_spacer|small||\n\nadd_spacer|small||\nadd_quick_exit|add_spacer|small||\nadd_url_button|comment|`2Update Available!``|noflags|https://ubistatic-a.akamaihd.net/0098/22809/GrowtopiaInstaller.exe|Download the new windows client now?|0|0||\nadd_spacer|small||\nadd_url_button|comment|`wVisit Growtopia Forums``|noflags|https://www.growtopiagame.com/forums|Visit the Growtopia forums?|0|0||\nadd_spacer|small||\nadd_url_button||`wWOTD: `1SONICDERBY`` by `#owley````|NOFLAGS|OPENWORLD|SONICDERBY|0|0||\nadd_spacer|small||\nadd_url_button||`wVOTW: `1S.W.A.T. - Hostage Rescue | Growtopia Animation``|NOFLAGS|https://www.youtube.com/watch?v=8Q7yYcAvWt8|Watch 'S.W.A.T. - Hostage Rescue | Growtopia Animation' by NATFLIXX on YouTube?|0|0||\nset_survey_enabled|0|\nend_dialog|gazette||OK|";
vector<string> nukedworlds;
string rules = "set_default_color|`o|\nadd_label_with_icon|big|`wHelp & Rules``|left|18||\nadd_spacer|small||\nadd_textbox|`wTo keep this place fun we've got some ground rules to check out:``|left||\nadd_spacer|small||\nadd_label_with_icon|small|`wKeep your password secret. Sharing your password will result in stolen items.``|left|24||\nadd_label_with_icon|small|`wUnprotected items are at risk of being stolen. Use doors, locks, and blocks wisely to protect your items.``|left|24||\nadd_label_with_icon|small|`wWe cannot return stolen items, so BE CAREFUL!``|left|24||\nadd_label_with_icon|small|`wBe civil. Bullying, racism, excessive profanity, sexual content, and abusive behavior are not allowed.``|left|24||\nadd_label_with_icon|small|`wPlayers that are harmful to the community may be banned. This includes accounts that use lies, fake games, or trickery to mistreat other players.``|left|24||\nadd_label_with_icon|small|`wPunishments may be applied to alt accounts as well as any devices and IP addresses used by the offender.``|left|24||\nadd_label_with_icon|small|`wTrying to get punished or asking for a punishment can earn you a worse punishment.``|left|24||\nadd_label_with_icon|small|`w\"Hacking\" the game - including client-side manipulation, auto-clickers, speed hacks, clock manipulation, bots, macroing, and autofarming - will result in a ban.``|left|24||\nadd_label_with_icon|small|`wDon't lie about mods or fake official Growtopia system messages.``|left|24||\nadd_label_with_icon|small|`w\"Drop games\" are illegal scams. You can be punished for hosting, playing, advertising or even watching.``|left|24||\nadd_label_with_icon|small|`wBetting or gambling (asking players to pay for a chance to win something) is not allowed.``|left|24||\nadd_label_with_icon|small|`wIf you find a world or player name that is in violation of our rules, message a @Mod. Do not /bc or /sb from an inappropriate world.``|left|24||\nadd_label_with_icon|small|`w@Moderators are here to enforce the rules. Abusing, spamming or harassing mods will have consequences.``|left|24||\nadd_label_with_icon|small|`wAccounts, locks, or worlds may be deleted after long periods of inactivity.``|left|24||\nadd_label_with_icon|small|`wOut of game buying/selling of Growtopia accounts, items or worlds is not allowed and may result in a ban.``|left|24||\nadd_label_with_icon|small|`wIAP fraud will result in permanent suspension.``|left|24||\nadd_label_with_icon|small|`wItem function and prices are subject to change without warning.``|left|24||\nadd_label_with_icon|small|`wYour creations and items could be deleted at any time. We do everything in our power to prevent this, but data loss may happen without warning. Lost items cannot be restored.``|left|24||\nadd_spacer|small||\nadd_label_with_icon|small|`wFor technical support, IAP issues, or to report a problem, please email support@growtopiagame.com``|left|24||\nadd_spacer|small||\nend_dialog|popup||Continue|";
/*

 |\n\nadd_spacer|small|\nadd_label_with_icon|small|`` `#New Feature:`w World nuking is now available. Note When a world is nuked, it will stay nuked until the server shuts down.|left|24||\n\nadd_spacer|small|\nadd_label_with_icon|small|`#New Feature:`w Moderators can also warn users for spamming. If they find the person spamming too much, they can also ducttape.|left|24|

*/

//#define TOTAL_LOG
#define REGISTRATION
#include <signal.h>
#ifdef __linux__
#include <cstdint>
typedef unsigned char BYTE;
typedef unsigned char byte;
typedef unsigned char __int8;
typedef unsigned short __int16;
typedef unsigned int DWORD;
#endif
ENetHost * server;
int cId = 1;
BYTE* itemsDat = 0;
int itemsDatSize = 0;
//Linux equivalent of GetLastError
#ifdef __linux__
string GetLastError() {
	return strerror(errno);
}
//Linux has no byteswap functions.
ulong _byteswap_ulong(ulong x)
{
	// swap adjacent 32-bit blocks
	//x = (x >> 32) | (x << 32);
	// swap adjacent 16-bit blocks
	x = ((x & 0xFFFF0000FFFF0000) >> 16) | ((x & 0x0000FFFF0000FFFF) << 16);
	// swap adjacent 8-bit blocks
	return ((x & 0xFF00FF00FF00FF00) >> 8) | ((x & 0x00FF00FF00FF00FF) << 8);
}
#endif

//configs
int configPort = 17091;
//46804
string configCDN = "0098/08794/cache/"; 
//string configCDN = "cache/";
int serverEvent = 0;//maintenance is 27
int serverMaintenance = 0;
/*
-------Monthly Events -----------
0 = None, 1 = Anniversary Week, 2 = Lunar New Year, 3 = Valentines week, 4 = St. Patricks Week, 5 = Easter Week,
6 = Cinco De Mayo week, 7 = Super Pineapple Week, 8 = Summerfest, 9 = P.A.W, 10 = Harvest Festival,
11 = Wing Week, 12 = Halloween, 13 = Thanksgiving, 14 = Winterfest,
-------Weekly/Daily Events-------
15 = Carnival, 16 = Night of The Comet, 17 = Locke the Travling Salesman, 18 = The Grand Tournament,
19 = Surgery Day, 20 = All howl's eve, 21 = Geiger Day, 22 = Ghost Day, 23 = Mutant Kitchen, 26 = Night of the Comet
--------------OTHER--------------
24 = Apology Week/Weekend, 25 = NUL, 27 = maintenance
*/

/*
serverMaintenance , 0 = disabled, 1 = enabled.
*/
//0098/12040/cache/

/***bcrypt***/

bool verifyPassword(string password, string hash) {
	int ret;
	
	 ret = bcrypt_checkpw(password.c_str(), hash.c_str());
	assert(ret != -1);
	
	return !ret;
}

bool has_only_digits(const string str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

string hashPassword(string password) {
	char salt[BCRYPT_HASHSIZE];
	char hash[BCRYPT_HASHSIZE];
	int ret;
	
	ret = bcrypt_gensalt(12, salt);
	assert(ret == 0);
	ret = bcrypt_hashpw(password.c_str(), salt, hash);
	assert(ret == 0);
	return hash;
}

/***bcrypt**/


void sendData(ENetPeer* peer, int num, char* data, int len)
{
	/* Create a reliable packet of size 7 containing "packet\0" */
	ENetPacket * packet = enet_packet_create(0,
		len + 5,
		ENET_PACKET_FLAG_RELIABLE);
	/* Extend the packet so and append the string "foo", so it now */
	/* contains "packetfoo\0"                                      */
	/* Send the packet to the peer over channel id 0. */
	/* One could also broadcast the packet by         */
	/* enet_host_broadcast (host, 0, packet);         */
	memcpy(packet->data, &num, 4);
	if (data != NULL)
	{
		memcpy(packet->data+4, data, len);
	}
	char zero = 0;
	memcpy(packet->data + 4 + len, &zero, 1);
	enet_peer_send(peer, 0, packet);
	enet_host_flush(server);
}

int getPacketId(char* data)
{
	return *data;
}

char* getPacketData(char* data)
{
	return data + 4;
}

string text_encode(char* text)
{
	string ret = "";
	while (text[0] != 0)
	{
		switch (text[0])
		{
		case '\n':
			ret += "\\n";
			break;
		case '\t':
			ret += "\\t";
			break;
		case '\b':
			ret += "\\b";
			break;
		case '\\':
			ret += "\\\\";
			break;
		case '\r':
			ret += "\\r";
			break;
		default:
			ret += text[0];
			break;
		}
		text++;
	}
	return ret;
}

int ch2n(char x)
{
	switch (x)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'A':
		return 10;
	case 'B':
		return 11;
	case 'C':
		return 12;
	case 'D':
		return 13;
	case 'E':
		return 14;
	case 'F':
		return 15;
	default:
		break;
	}
}


char* GetTextPointerFromPacket(ENetPacket* packet)
{
	char zero = 0;
	memcpy(packet->data + packet->dataLength - 1, &zero, 1);
	return (char*)(packet->data + 4);
}

BYTE* GetStructPointerFromTankPacket(ENetPacket* packet)
{
	unsigned int packetLenght = packet->dataLength;
	BYTE* result = NULL;
	if (packetLenght >= 0x3C)
	{
		BYTE* packetData = packet->data;
		result = packetData + 4;
		if (*(BYTE*)(packetData + 16) & 8)
		{
			if (packetLenght < *(int*)(packetData + 56) + 60)
			{
				cout << "Packet too small for extended packet to be valid" << endl;
				cout << "Sizeof float is 4.  TankUpdatePacket size: 56" << endl;
				result = 0;
			}
		}
		else
		{
			int zero = 0;
			memcpy(packetData + 56, &zero, 4);
		}
	}
	return result;
}

int GetMessageTypeFromPacket(ENetPacket* packet)
{
	int result;

	if (packet->dataLength > 3u)
	{
		result = *(packet->data);
	}
	else
	{
		cout << "Bad packet length, ignoring message" << endl;
		result = 0;
	}
	return result;
}


vector<string> explode(const string &delimiter, const string &str)
{
	vector<string> arr;

	int strleng = str.length();
	int delleng = delimiter.length();
	if (delleng == 0)
		return arr;//no change

	int i = 0;
	int k = 0;
	while (i<strleng)
	{
		int j = 0;
		while (i + j<strleng && j<delleng && str[i + j] == delimiter[j])
			j++;
		if (j == delleng)//found delimiter
		{
			arr.push_back(str.substr(k, i - k));
			i += delleng;
			k = i;
		}
		else
		{
			i++;
		}
	}
	arr.push_back(str.substr(k, i - k));
	return arr;
}

struct gamepacket_t
{
private:
	int index = 0;
	int len = 0;
	byte* packet_data = new byte[61];

public:
	gamepacket_t(int delay = 0, int NetID = -1) {

		len = 61;
		int MessageType = 0x4;
		int PacketType = 0x1;
		int CharState = 0x8;

		memset(packet_data, 0, 61);
		memcpy(packet_data, &MessageType, 4);
		memcpy(packet_data + 4, &PacketType, 4);
		memcpy(packet_data + 8, &NetID, 4);
		memcpy(packet_data + 16, &CharState, 4);
		memcpy(packet_data + 24, &delay, 4);
	};
	~gamepacket_t() {
		delete[] packet_data;
	}

	void Insert(string a) {
		byte* data = new byte[len + 2 + a.length() + 4];
		memcpy(data, packet_data, len);
		delete[] packet_data;
		packet_data = data;
		data[len] = index;
		data[len + 1] = 0x2;
		int str_len = a.length();
		memcpy(data + len + 2, &str_len, 4);
		memcpy(data + len + 6, a.data(), str_len);
		len = len + 2 + a.length() + 4;
		index++;
		packet_data[60] = (byte)index;
	}
	void Insert(int a) {
		byte* data = new byte[len + 2 + 4];
		memcpy(data, packet_data, len);
		delete[] packet_data;
		packet_data = data;
		data[len] = index;
		data[len + 1] = 0x9;
		memcpy(data + len + 2, &a, 4);
		len = len + 2 + 4;
		index++;
		packet_data[60] = (byte)index;
	}
	void Insert(unsigned int a) {
		byte* data = new byte[len + 2 + 4];
		memcpy(data, packet_data, len);
		delete[] packet_data;
		packet_data = data;
		data[len] = index;
		data[len + 1] = 0x5;
		memcpy(data + len + 2, &a, 4);
		len = len + 2 + 4;
		index++;
		packet_data[60] = (byte)index;
	}
	void Insert(float a) {
		byte* data = new byte[len + 2 + 4];
		memcpy(data, packet_data, len);
		delete[] packet_data;
		packet_data = data;
		data[len] = index;
		data[len + 1] = 0x1;
		memcpy(data + len + 2, &a, 4);
		len = len + 2 + 4;
		index++;
		packet_data[60] = (byte)index;
	}
	void Insert(float a, float b) {
		byte* data = new byte[len + 2 + 8];
		memcpy(data, packet_data, len);
		delete[] packet_data;
		packet_data = data;
		data[len] = index;
		data[len + 1] = 0x3;
		memcpy(data + len + 2, &a, 4);
		memcpy(data + len + 6, &b, 4);
		len = len + 2 + 8;
		index++;
		packet_data[60] = (byte)index;
	}
	void Insert(float a, float b, float c) {
		byte* data = new byte[len + 2 + 12];
		memcpy(data, packet_data, len);
		delete[] packet_data;
		packet_data = data;
		data[len] = index;
		data[len + 1] = 0x4;
		memcpy(data + len + 2, &a, 4);
		memcpy(data + len + 6, &b, 4);
		memcpy(data + len + 10, &c, 4);
		len = len + 2 + 12;
		index++;
		packet_data[60] = (byte)index;
	}
	void CreatePacket(ENetPeer* peer) {
		ENetPacket* packet = enet_packet_create(packet_data, len, 1);
		enet_peer_send(peer, 0, packet);
	}
};

struct ItemSharedUID {
	int actual_uid = 1;
	int shared_uid = 1;
};

struct InventoryItem {
	__int16 itemID;
	__int8 itemCount;
};

struct PlayerInventory {
	vector<InventoryItem> items;
	int inventorySize = 100;
};

#define cloth0 cloth_hair
#define cloth1 cloth_shirt
#define cloth2 cloth_pants
#define cloth3 cloth_feet
#define cloth4 cloth_face
#define cloth5 cloth_hand
#define cloth6 cloth_back
#define cloth7 cloth_mask
#define cloth8 cloth_necklace
#define cloth9 cloth_ances

struct PlayerInfo {
	string lastMsger = "";
	string lastMsgerTrue = "";
	string lastMsgWorld = "";
	bool hidden = false;
	bool isIn = false;
	int effect = 0;
	int level = 1;
	int xp = 0;
	int gems = 0;
	string tankIDName = "";
	string tankIDPass = "";
	string tankIDEmail = "";
	string currentWorld = "EXIT";
	bool radio = true;
	int x;
	int y;
	int x1;
	int y1;
	bool isRotatedLeft = false;
	string charIP = "";
	bool isUpdating = false;
	bool joinClothesUpdated = false;

	bool hasLogon = false;
	bool test = false;

	//Player Info
	int netID;
	bool haveGrowId = false;
	string requestedName = "";
	string rawName = "";
	string displayName = "";
	string country = "";
	string pactPass = "";
	string gameversion = "";
	string platformID = "";
	string deviceversion = "";
	string rid = "";

	//Punishments
	bool taped = false;
	bool isSuspended = false;
	bool isDuctaped = false; // state 8192
	bool isCursed = false; // state 4096

	int playerWarnings = 0;

	//Moderator stuff
	bool isInvisibles = false;
	int adminLevel = 0;
	bool canWalkInBlocks = false; // state 1
	bool isInvisible = false; // state 4
	bool isGhost = false;

	//Just extra cool stuff
	bool haveSuperSupporterName = false; // state 16777216
	int characterState = 0;
	//Player Clothing
	int cloth_hair = 0; // 0
	int cloth_shirt = 0; // 1
	int cloth_pants = 0; // 2
	int cloth_feet = 0; // 3
	int cloth_face = 0; // 4
	int cloth_hand = 0; // 5
	int cloth_back = 0; // 6
	int cloth_mask = 0; // 7
	int cloth_necklace = 0; // 8
	int cloth_ances = 0; // 9

	int hair_actual = 0;
	int shirt_actual = 0;
	int pants_actual = 0;
	int feet_actual = 0;
	int face_actual = 0;
	int hand_actual = 0;
	int back_actual = 0;
	int mask_actual = 0;
	int necklace_actual = 0;
	int ances_actual = 0;

	//PLayer States
	bool canDoubleJump = false; // 2
	bool noHands = false; // 8
	bool noEyes = false; // 16
	bool noBody = false; // 32
	bool devilHorns = false; // 64
	bool goldenHalo = false; // 128
	bool isFrozen = false; // 2048
	bool haveCigar = false; // 16384
	bool isShining = false; // 32768
	bool isZombie = false; // 65536
	bool isHitByLava = false; // 131072
	bool haveHauntedShadows = false; // 262144
	bool haveGeigerRadiation = false; // 524288
	bool haveReflector = false; // 1048576
	bool isEgged = false; // 2097152
	bool havePineappleFloag = false; // 4194304
	bool haveFlyingPineapple = false; // 8388608
	bool haveSupperPineapple = false; // 33554432

    // shop shit and legendary shit
	//legen items
	bool hasLegenName = false;
	bool hasLegenDrag = false;
	bool hasLegenBot = false;
	bool hasLegenWing = false;
	bool hasLegenKatana = false;
	bool hasLegenWhip = false;
	bool hasLegenKnight = false;
	//rings
	bool hasFistfulRing = false;
	bool hasOneRing = false;
	bool hasGeminiRing = false;
	bool hasForceRing = false;
	bool hasWindRing = false;
	bool hasWisdomRing = false;
	bool hasWaterRing = false;
	bool hasSmithRing = false;
	bool hasShrinkRing = false;
	bool hasNatureRing = false;
	bool hasNightRing = false;
	//iotms
	bool hasCosmo = false;
	bool hasPirate = false;
	bool hasBalloon = false;
	bool hasRocketWing = false;

	//mod/dev clothes
	bool hasHeroman = false;
	bool hasNekorei = false;
	bool hasApollo = false;
	bool hasSolo = false;
	bool hasKuma = false;
	bool hasSaturday = false;
	bool hasGenuine = false;
	bool hasNightmare = false;
	bool hasOwlBeard = false;
	bool hasArch = false;
	bool hasUnity = false;
	bool hasCult = false;
	bool hasMighty = false;
	bool hasRedBone = false;
	bool hasBurning = false;
	bool hasPlay = false;
	bool hasDemonic = false;
	bool hasGreenswe = false;
	bool hasTony = false;
	bool hasWitch = false;
	bool hasMeh = false;
	bool hasWobbly = false;
	bool hasWaterSpirit = false;
	bool hasLokster = false;
	bool hasSolsa = false;
	bool hasShadow = false;
	bool hasJens = false;


	// ITEM EFFECTS
		
	bool isFrozenEffect = false;
	int isFrozenEffectCounter = 0;
		

	////////////////
	string lastInfo = "";
	string lastInfoname = "";

	//bool 
	int skinColor = 0x8295C3FF; //normal SKin color like gt!

	PlayerInventory inventory;

	long long int lastSB = 0;

	//hacky dropped item stuff :(
	vector<ItemSharedUID> item_uids;
	int last_uid = 1;
};


int getState(PlayerInfo* info) {
	int val = 0;
	val |= info->canWalkInBlocks << 0;
	val |= info->canDoubleJump << 1;
	val |= info->isInvisible << 2;
	val |= info->noHands << 3;
	val |= info->noEyes << 4;
	val |= info->noBody << 5;
	val |= info->devilHorns << 6;
	val |= info->goldenHalo << 7;
	val |= info->isFrozen << 11;
	val |= info->isCursed << 12;
	val |= info->isDuctaped << 13;
	val |= info->haveCigar << 14;
	val |= info->isShining << 15;
	val |= info->isZombie << 16;
	val |= info->isHitByLava << 17;
	val |= info->haveHauntedShadows << 18;
	val |= info->haveGeigerRadiation << 19;
	val |= info->haveReflector << 20;
	val |= info->isEgged << 21;
	val |= info->havePineappleFloag << 22;
	val |= info->haveFlyingPineapple << 23;
	val |= info->haveSuperSupporterName << 24;
	val |= info->haveSupperPineapple << 25;
	return val;
}


struct WorldItem {
	__int16 foreground = 0;
	__int16 background = 0;
	int breakLevel = 0;
	long long int breakTime = 0;
	bool water = false;
	bool fire = false;
	bool glue = false;
	bool red = false;
	bool green = false;
	bool blue = false;

};

struct DroppedItem { // TODO
	int id;
	int uid;
	int count;
	int x;
	int y;
};

struct WorldInfo {
	int width = 100;
	int height = 60;
	string admins = "";
	string name = "TEST";
	int weather = 0;
	
	WorldItem* items;
	string owner = "";
	bool isPublic= false;

	unsigned long currentItemUID = 1; //has to be 1 by default
	vector<DroppedItem> droppedItems;
};

WorldInfo generateWorld(string name, int width, int height)
{
	WorldInfo world;
	world.name = name;
	world.width = width;
	world.height = height;
	world.items = new WorldItem[world.width*world.height];
	for (int i = 0; i < world.width*world.height; i++)
	{
		if (i >= 3800 && i < 5400 && !(rand() % 50)){ world.items[i].foreground = 10; }
		else if (i >= 3700 && i < 5400) {
			if(i > 5000) {
				if (i % 7 == 0) { world.items[i].foreground = 4;}
				else { world.items[i].foreground = 2; }
			}
			else { world.items[i].foreground = 2; }
		}
		else if (i >= 5400) { world.items[i].foreground = 8; }
		if (i >= 3700)
			world.items[i].background = 14;
		if (i == 3650)
			world.items[i].foreground = 6;
		else if (i >= 3600 && i<3700)
			world.items[i].foreground = 0; //fixed the grass in the world!
		if (i == 3750)
			world.items[i].foreground = 8;
	}
	return world;
}

WorldInfo generateBeachWorld(string name, int width, int height)
{
	WorldInfo world;
	world.name = name;
	world.width = width;
	world.height = height;
	world.weather = 1;
	world.items = new WorldItem[world.width*world.height];
	for (int i = 0; i < world.width*world.height; i++)
	{
		if (i >= 3800 && i < 5400 && !(rand() % 50)) { world.items[i].foreground = 10; }
		else if (i >= 3700 && i < 5400) {
			if (i > 5000) {
				if (i % 7 == 0) { world.items[i].foreground = 4; }
				else { world.items[i].foreground = 442; }
			}
			else { world.items[i].foreground = 442; }
		}
		else if (i >= 5400) { world.items[i].foreground = 8; }
		if (i >= 3700)
			world.items[i].background = 850;
		if (i == 3650)
			world.items[i].foreground = 6;
		else if (i >= 3600 && i < 3700)
			world.items[i].foreground = 0; //fixed the grass in the world!
		if (i == 3750)
			world.items[i].foreground = 8;
	}
	return world;
}


class PlayerDB {
public:
	static string getProperName(string name);
	static string fixColors(string text);
	static void pInven(ENetPeer* peer, string username);
	static int playerLogin(ENetPeer* peer, string username, string password, bool isSuspended);
	static int playerRegister(ENetPeer * peer, string username, string password, string passwordverify, string email);
	
};
void PlayerDB::pInven(ENetPeer* peer, string username)
{
	ofstream o("inven/" + username + ".json");
	json j;
	PlayerInventory inventory = ((PlayerInfo*)(peer->data))->inventory;
	int isize = inventory.items.size();
	j["isize"] = isize;
	json inven = json::array();
	for (int a = 0; a < isize; a++)
	{
		json inv;
		inv["count"] = inventory.items[a].itemCount;
		inv["id"] = inventory.items[a].itemID;
		inven.push_back(inv);
	}
	j["inv"] = inven;
	o << j << std::endl;
}

string PlayerDB::getProperName(string name) {
	string newS;
	for (char c : name) newS+=(c >= 'A' && c <= 'Z') ? c-('A'-'a') : c;
	string ret;
	for (int i = 0; i < newS.length(); i++)
	{
		if (newS[i] == '`') i++; else ret += newS[i];
	}
	string ret2;
	for (char c : ret) if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) ret2 += c;
	
	string username = ret2;
	if (username == "prn" || username == "con" || username == "aux" || username == "nul" || username == "com1" || username == "com2" || username == "com3" || username == "com4" || username == "com5" || username == "com6" || username == "com7" || username == "com8" || username == "com9" || username == "lpt1" || username == "lpt2" || username == "lpt3" || username == "lpt4" || username == "lpt5" || username == "lpt6" || username == "lpt7" || username == "lpt8" || username == "lpt9") {
		return "";
	}
	
	return ret2;
}

string PlayerDB::fixColors(string text) {
	string ret = "";
	int colorLevel = 0;
	for (int i = 0; i < text.length(); i++)
	{
		if (text[i] == '`')
		{
			ret += text[i];
			if (i + 1 < text.length())
				ret += text[i + 1];
			
			
			if (i+1 < text.length() && text[i + 1] == '`')
			{
				colorLevel--;
			}
			else {
				colorLevel++;
			}
			i++;
		} else {
			ret += text[i];
		}
	}
	for (int i = 0; i < colorLevel; i++) {
		ret += "``";
	}
	for (int i = 0; i > colorLevel; i--) {
		ret += "`w";
	}
	return ret;
}

struct Admin {
	string username;
	string password;
	int level = 0;
	long long int lastSB = 0;
};

vector<Admin> admins;
void passBad(ENetPeer* peer) {
	//				p3.Insert("`4Sorry this account (`5" + ((PlayerInfo*)(peer->data))->rawName + "`4) has been suspended. Contact `5https://support.ubi.com/en-GB/games/5847 `4if you have any questions.");

	string text = "action|log\nmsg|`4Unable to log on:`o That `wGrowID`o doesn't seem valid, or the password is wrong. If you don't have one, press `wCancel`o, un-check `w'I have a GrowID'`o, then click `wConnect`o.";
	string text2 = "action|set_url\nurl|https://growtopiagame.com\nlabel|`$Retrive lost password\n";
	string text3 = "action|logon_fail\n";

	BYTE* data = new BYTE[5 + text.length()];
	BYTE* data2 = new BYTE[5 + text2.length()];
	BYTE* data3 = new BYTE[5 + text3.length()];

	BYTE zero = 0;
	int type = 3;
	memcpy(data, &type, 4);
	memcpy(data + 4, text.c_str(), text.length());
	memcpy(data + 4 + text.length(), &zero, 1);

	memcpy(data2, &type, 4);
	memcpy(data2 + 4, text2.c_str(), text2.length());
	memcpy(data2 + 4 + text2.length(), &zero, 1);

	memcpy(data3, &type, 4);
	memcpy(data3 + 4, text3.c_str(), text3.length());
	memcpy(data3 + 4 + text3.length(), &zero, 1);

	ENetPacket* p = enet_packet_create(data,
		5 + text.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p);

	ENetPacket* packet5 = enet_packet_create(data2,
		5 + text2.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet5);

	ENetPacket* p2 = enet_packet_create(data3,
		5 + text3.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p2);

	delete data;
	delete data2;
	delete data3;

	enet_peer_disconnect_later(peer, 0);
}
void maintlog(ENetPeer* peer) {

	string text = "action|log\nmsg|`5Sorry, The server is currently under maintenance.";
	string text2 = "action|set_url\nurl|https://growtopiagame.com/forums\nlabel|`$Growtopia Forums\n";
	string text3 = "action|logon_fail\n";

	BYTE* data = new BYTE[5 + text.length()];
	BYTE* data2 = new BYTE[5 + text2.length()];
	BYTE* data3 = new BYTE[5 + text3.length()];

	BYTE zero = 0;
	int type = 3;
	memcpy(data, &type, 4);
	memcpy(data + 4, text.c_str(), text.length());
	memcpy(data + 4 + text.length(), &zero, 1);

	memcpy(data2, &type, 4);
	memcpy(data2 + 4, text2.c_str(), text2.length());
	memcpy(data2 + 4 + text2.length(), &zero, 1);

	memcpy(data3, &type, 4);
	memcpy(data3 + 4, text3.c_str(), text3.length());
	memcpy(data3 + 4 + text3.length(), &zero, 1);

	ENetPacket* p = enet_packet_create(data,
		5 + text.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p);

	ENetPacket* packet5 = enet_packet_create(data2,
		5 + text2.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet5);

	ENetPacket* p2 = enet_packet_create(data3,
		5 + text3.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p2);

	delete data;
	delete data2;
	delete data3;

	enet_peer_disconnect_later(peer, 0);
}
void updatelog(ENetPeer* peer) {
	string text = "action|log\nmsg|`4UPDATE REQUIRED`o : The `$V3.50 `oupdate is now avallable for your device. Go get it! You'll need that before you can login!\n";
	string text2 = "action|set_url\nurl|https://growtopiagame.com\nlabel|`$Update Growtopia!\n";
	string text3 = "action|logon_fail\n";

	BYTE* data = new BYTE[5 + text.length()];
	BYTE* data2 = new BYTE[5 + text2.length()];
	BYTE* data3 = new BYTE[5 + text3.length()];

	BYTE zero = 0;
	int type = 3;
	memcpy(data, &type, 4);
	memcpy(data + 4, text.c_str(), text.length());
	memcpy(data + 4 + text.length(), &zero, 1);

	memcpy(data2, &type, 4);
	memcpy(data2 + 4, text2.c_str(), text2.length());
	memcpy(data2 + 4 + text2.length(), &zero, 1);

	memcpy(data3, &type, 4);
	memcpy(data3 + 4, text3.c_str(), text3.length());
	memcpy(data3 + 4 + text3.length(), &zero, 1);

	ENetPacket* p = enet_packet_create(data,
		5 + text.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p);

	ENetPacket* packet5 = enet_packet_create(data2,
		5 + text2.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet5);

	ENetPacket* p2 = enet_packet_create(data3,
		5 + text3.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p2);

	delete data;
	delete data2;
	delete data3;

	enet_peer_disconnect_later(peer, 0);
}
void banlogin(ENetPeer* peer) {
	//				p3.Insert("`4Sorry this account (`5" + ((PlayerInfo*)(peer->data))->rawName + "`4) has been suspended. Contact `5https://support.ubi.com/en-GB/games/5847 `4if you have any questions.");
	
	string text = "action|log\nmsg|`4Sorry, this account (`5" + ((PlayerInfo*)(peer->data))->rawName + "`4) has been suspended. Contact `5https://support.ubi.com/en-GB/games/5847 `4if you have any questions.";
	string text3 = "action|logon_fail\n";
	
	BYTE* data = new BYTE[5 + text.length()];
	BYTE* data3 = new BYTE[5 + text3.length()];
	
	BYTE zero = 0;
	int type = 3;
	memcpy(data, &type, 4);
	memcpy(data + 4, text.c_str(), text.length());
	memcpy(data + 4 + text.length(), &zero, 1);



	memcpy(data3, &type, 4);
	memcpy(data3 + 4, text3.c_str(), text3.length());
	memcpy(data3 + 4 + text3.length(), &zero, 1);

	ENetPacket* p = enet_packet_create(data,
		5 + text.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p);
	
	ENetPacket* p2 = enet_packet_create(data3,
		5 + text3.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p2);

	delete data;
	delete data3;

	enet_peer_disconnect_later(peer, 0);
}
void badChar(ENetPeer* peer) {

	string text = "action|log\nmsg|`4Sorry!`o You can not use special characters in your name! Please disconnect and change your name.";
	string text3 = "action|logon_fail\n";

	BYTE* data = new BYTE[5 + text.length()];
	BYTE* data3 = new BYTE[5 + text3.length()];

	BYTE zero = 0;
	int type = 3;
	memcpy(data, &type, 4);
	memcpy(data + 4, text.c_str(), text.length());
	memcpy(data + 4 + text.length(), &zero, 1);



	memcpy(data3, &type, 4);
	memcpy(data3 + 4, text3.c_str(), text3.length());
	memcpy(data3 + 4 + text3.length(), &zero, 1);

	ENetPacket* p = enet_packet_create(data,
		5 + text.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p);

	ENetPacket* p2 = enet_packet_create(data3,
		5 + text3.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p2);

	delete data;
	delete data3;

	enet_peer_disconnect_later(peer, 0);
}
void maintenance(ENetPeer* peer) {
	//				p3.Insert("`4Sorry this account (`5" + ((PlayerInfo*)(peer->data))->rawName + "`4) has been suspended. Contact `5https://support.ubi.com/en-GB/games/5847 `4if you have any questions.");

	string text = "action|log\nmsg|`5Sorry the server is currently down for maintenance, we will be back shortly.";
	string text3 = "action|logon_fail\n";

	BYTE* data = new BYTE[5 + text.length()];
	BYTE* data3 = new BYTE[5 + text3.length()];

	BYTE zero = 0;
	int type = 3;
	memcpy(data, &type, 4);
	memcpy(data + 4, text.c_str(), text.length());
	memcpy(data + 4 + text.length(), &zero, 1);



	memcpy(data3, &type, 4);
	memcpy(data3 + 4, text3.c_str(), text3.length());
	memcpy(data3 + 4 + text3.length(), &zero, 1);

	ENetPacket* p = enet_packet_create(data,
		5 + text.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p);

	ENetPacket* p2 = enet_packet_create(data3,
		5 + text3.length(),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p2);

	delete data;
	delete data3;

	enet_peer_disconnect_later(peer, 0);
}
int PlayerDB::playerLogin(ENetPeer* peer, string username, string password, bool isSuspended) {
	std::ifstream ifs("players/" + PlayerDB::getProperName(username) + ".json");
	if (ifs.is_open()) {
		json j;
		ifs >> j;
		string pss = j["password"];
		int adminLevel = j["adminLevel"];
		bool suspended = j["isSuspended"];
	    int back = j["ClothBack"];
		int face = j["ClothFace"];
		int feet = j["ClothFeet"];
		int hair = j["ClothHair"];
		int hand = j["ClothHand"];
		int mask = j["ClothMask"];
		int neck = j["ClothNeck"];
		int pants = j["ClothPants"];
		int shirt = j["ClothShirt"];
		int gems = j["gems"];
		int level = j["level"];
		((PlayerInfo*)(peer->data))->adminLevel = adminLevel;
		((PlayerInfo*)(peer->data))->playerWarnings = j["playerWarnings"];
		((PlayerInfo*)(peer->data))->hasLegenName = j["hasLegenName"];
		((PlayerInfo*)(peer->data))->hasLegenDrag = j["hasLegenDrag"];
		((PlayerInfo*)(peer->data))->hasLegenBot = j["hasLegenBot"];
		((PlayerInfo*)(peer->data))->hasLegenWing = j["hasLegenWing"];
		((PlayerInfo*)(peer->data))->hasLegenKatana = j["hasLegenKatana"];
		((PlayerInfo*)(peer->data))->hasLegenWhip = j["hasLegenWhip"];
		((PlayerInfo*)(peer->data))->hasLegenKnight = j["hasLegenKnight"];

		string emails = j["email"];
		bool didSomethingHappen = false;
	//calvin
		if (verifyPassword(password, pss)) {
			//main user stuffs
			((PlayerInfo*)(peer->data))->hasLogon = true;
			((PlayerInfo*)(peer->data))->tankIDEmail = emails;
			((PlayerInfo*)(peer->data))->pactPass = pss;
			((PlayerInfo*)(peer->data))->isSuspended = suspended;
			
			//clothes 
			((PlayerInfo*)(peer->data))->cloth_back = back;
			((PlayerInfo*)(peer->data))->cloth_face = face;
			((PlayerInfo*)(peer->data))->cloth_feet = feet;
			((PlayerInfo*)(peer->data))->cloth_hair = hair;
			((PlayerInfo*)(peer->data))->cloth_hand = hand;
			((PlayerInfo*)(peer->data))->cloth_mask = mask;
			((PlayerInfo*)(peer->data))->cloth_necklace = neck;
			((PlayerInfo*)(peer->data))->cloth_pants = pants;
			((PlayerInfo*)(peer->data))->cloth_shirt = shirt;
			((PlayerInfo*)(peer->data))->gems = gems;
			((PlayerInfo*)(peer->data))->level = level;
		
			//items

			//actual this is only used for mods really.
			((PlayerInfo*)(peer->data))->back_actual = back;
			((PlayerInfo*)(peer->data))->face_actual = face;
			((PlayerInfo*)(peer->data))->feet_actual = feet;
			((PlayerInfo*)(peer->data))->hair_actual = hair;
			((PlayerInfo*)(peer->data))->hand_actual = hand;
			((PlayerInfo*)(peer->data))->mask_actual = mask;
			((PlayerInfo*)(peer->data))->necklace_actual = neck;
			((PlayerInfo*)(peer->data))->pants_actual = pants;
			((PlayerInfo*)(peer->data))->shirt_actual = shirt;
			

			
			//after verify password add adminlevel not before
			//((PlayerInfo*)(peer->data))->isSuspended
			if (suspended == true) {
				/*cout << "suspended player " + username + " logged on";
				gamepacket_t p3;
				p3.Insert("OnConsoleMessage");
				p3.Insert("`4Sorry this account (`5" + ((PlayerInfo*)(peer->data))->rawName + "`4) has been suspended. Contact `5https://support.ubi.com/en-GB/games/5847 `4if you have any questions.");
				p3.CreatePacket(peer);*/
				cout << "Account: " << ((PlayerInfo*)(peer->data))->rawName << " is suspended and is trying to enter the game\n";
				//enet_peer_disconnect_later(peer, 0);
				banlogin(peer);
			}
			
			/*if (((PlayerInfo*)(peer->data))->gameversion != "3.61") {
				updatelog(peer);
				cout << ((PlayerInfo*)(peer->data))->gameversion << endl;
			}*/
			if (serverEvent == 27) {
				if (((PlayerInfo*)(peer->data))->rawName == "matty" || ((PlayerInfo*)(peer->data))->rawName == "seth" || ((PlayerInfo*)(peer->data))->rawName == "hamumu" || ((PlayerInfo*)(peer->data))->rawName == "ubidev") {
				//do nothing
				}
				else {
					maintlog(peer);
				}
				
			}
		
			string s1 = ((PlayerInfo*)(peer->data))->tankIDName;
			string s2 = "`";
			string s3 = "";
			if (s1.find("`" || "@" || "!" || "#" || "$" || "%" || "^" || "&" || "*" || "(" || ")" || "-" || "+" || "{" || "}" || "[" || "]" || ";" || ":" || "'" || "," || "." || "/" || "?" || "<" || ">" || "~") != std::string::npos) {
				badChar(peer);
			}
			if (s1.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
			{
				badChar(peer);
			}

			bool found = false;
			for (int i = 0; i < admins.size(); i++) {
				if (admins[i].username == username) {
				found = true;	
				}
			}
			if (!found) {//not in vector
				if (adminLevel != 0) {
					Admin admin;
					admin.username = PlayerDB::getProperName(username);
					admin.password = pss;
					admin.level = adminLevel;
					admins.push_back(admin);
				}
			}
			if (adminLevel != 0) {
				Admin admin;
				admin.username = PlayerDB::getProperName(username);
				admin.password = pss;
				admin.level = adminLevel;
				admins.push_back(admin);
				
			}
			ENetPeer * currentPeer;

			for (currentPeer = server->peers;
				currentPeer < &server->peers[server->peerCount];
				++currentPeer)
			{
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
					continue;
				if (currentPeer == peer)
					continue;
				
				if (((PlayerInfo*)(currentPeer->data))->rawName == PlayerDB::getProperName(username))
				{
					
					

					
					{
						gamepacket_t p;
						p.Insert("OnConsoleMessage");
						p.Insert("Someone else logged into this account!");
						p.CreatePacket(peer);
						enet_peer_disconnect_later(currentPeer, 0);
					}
					{
						gamepacket_t p;
						p.Insert("OnConsoleMessage");
						p.Insert("Someone else was logged into this account! He was kicked out now.");
						p.CreatePacket(peer);
						enet_peer_disconnect_later(currentPeer, 0);
					}
					
				
				}
			}
			return 1;
		}
		else {
			return -1;
		}
	}
	else {
		return -2;
	}
}/*
 if (getItemDef(tile).blockType == BlockTypes::CONSUMMABLE) {
			for (int i = 0; i < ((PlayerInfo*)(peer->data))->inventory.items.size(); i++)
			{
				if (((PlayerInfo*)(peer->data))->inventory.items.at(i).itemID == tile)
				{
					if ((unsigned int)((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount > 1)
					{
						((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount--;
					}
					else {
						((PlayerInfo*)(peer->data))->inventory.items.erase(((PlayerInfo*)(peer->data))->inventory.items.begin() + i);

					}
				}
			}*/


int PlayerDB::playerRegister(ENetPeer * peer, string username, string password, string passwordverify, string email) {
	//ENetPeer * peer;
	string name = username;
	int playerWarnings = 0;
    if (name == "CON" || name == "PRN" || name == "AUX" || name == "NUL" || name == "COM1" || name == "COM2" || name == "COM3" || name == "COM4" || name == "COM5" || name == "COM6" || name == "COM7" || name == "COM8" || name == "COM9" || name == "LPT1" || name == "LPT2" || name == "LPT3" || name == "LPT4" || name == "LPT5" || name == "LPT6" || name == "LPT7" || name == "LPT8" || name == "LPT9") return -1;
    username = PlayerDB::getProperName(username);
//	PlayerDB::pInven(peer, username);
    if (email.find("@") == std::string::npos && email.length() != 0) return -4;
    if (passwordverify != password) return -3;
    if (username.length() < 3) return -2;
	string s1 = username;
	string s2 = "`";
	if (s1.find("`" || "@" || "!" || "#" || "$" || "%" || "^" || "&" || "*" || "(" || ")" || "-" || "+" || "{" || "}" || "[" || "]" || ";" || ":" || "'" || "," || "." || "/" || "?" || "<" || ">" || "~") != std::string::npos) return -1;
	
    std::ifstream ifs("players/" + username + ".json");
    if (ifs.is_open()) {
        return -1;
    }
	string s3 = username;
	string s4 = "of Legend";
	if (s3.find(" Legend") != std::string::npos) return -1;

	
	std::ofstream o("players/" + username + ".json");
	if (!o.is_open()) {
		cout << GetLastError() << endl;
		_getch();
	}
	
	json j;
	j["username"] = username;
	j["password"] = hashPassword(password);
	j["email"] = email;
	j["adminLevel"] = 0;
	j["playerWarnings"] = 0;
	j["isSuspended"] = false;
	j["ClothBack"] = 0;
	j["ClothHand"] = 0;
	j["ClothFace"] = 0;
	j["ClothShirt"] = 0;
	j["ClothPants"] = 0;
	j["ClothNeck"] = 0;
	j["ClothHair"] = 0;
	j["ClothFeet"] = 0;
	j["ClothMask"] = 0;
	j["level"] = 1;
	j["gems"] = 0;
	j["xp"] = 0;
	j["hasLegenName"] = false;
	j["hasLegenDrag"] = false;
	j["hasLegenBot"] = false;
	j["hasLegenWing"] = false;
	j["hasLegenKatana"] = false;
	j["hasLegenWhip"] = false;
	j["hasLegenKnight"] = false;
	
	PlayerDB::pInven(peer, username);
	o << j << std::endl;
	
	return 1;
}


struct AWorld {
	WorldInfo* ptr;
	WorldInfo info;
	int id;
};

class WorldDB {
public:
	WorldInfo get(string name);
	AWorld get2(string name);
	void flush(WorldInfo info);
	void flush2(AWorld info);
	void save(AWorld info);
	void saveAll();
	void saveRedundant();
	vector<WorldInfo> getRandomWorlds();
	WorldDB();
private:
	vector<WorldInfo> worlds;
};

WorldDB::WorldDB() {
	// Constructor
}

namespace packet {
	void consolemessage(ENetPeer* peer, string message) {
		gamepacket_t p;
		p.Insert("OnConsoleMessage");
		p.Insert(message);
		p.CreatePacket(peer);
	}
	void dialog(ENetPeer* peer, string message) {
		gamepacket_t p;
		p.Insert("OnDialogRequest");
		p.Insert(message);
		p.CreatePacket(peer);
	}
	void onspawn(ENetPeer* peer, string message) {
		gamepacket_t p;
		p.Insert("OnSpawn");
		p.Insert(message);
		p.CreatePacket(peer);
	}
	void requestworldselectmenu(ENetPeer* peer, string message) {
		gamepacket_t p;
		p.Insert("OnRequestWorldSelectMenu");
		p.Insert(message);
		p.CreatePacket(peer);
	}
	void storerequest(ENetPeer* peer, string message) {
		gamepacket_t p;
		p.Insert("OnStoreRequest");
		p.Insert(message);
		p.CreatePacket(peer);
	}
	void storepurchaseresult(ENetPeer* peer, string message) {
		gamepacket_t p;
		p.Insert("OnStorePurchaseResult");
		p.Insert(message);
		p.CreatePacket(peer);
	}
	void setbux(ENetPeer * peer, int gems) {
		gamepacket_t p1;
		p1.Insert("OnSetBux");
		p1.Insert(gems);
		p1.CreatePacket(peer);
	}
	void tradestatus(ENetPeer * peer, int uid, int no, string username, int locked, int locks, int accepted) {
		gamepacket_t p1;
		p1.Insert("OnTradeStatus");
		p1.Insert(uid);
		p1.Insert(no);
		p1.Insert("`o" + username + "`o's offer.");
		p1.Insert("locked|" + std::to_string(locked) + "\nrest_locks|" + std::to_string(locks) + "\naccepted|" + std::to_string(accepted));
		p1.CreatePacket(peer);
	}
	void sendBoot(ENetPeer * peer) {
		enet_peer_disconnect_later(peer, 0);
	}
}

string getStrUpper(string txt) {
	string ret;
	for (char c : txt) ret += toupper(c);
	return ret;
}

AWorld WorldDB::get2(string name) {
	if (worlds.size() > 200) {
#ifdef TOTAL_LOG
		cout << "Saving redundant worlds!" << endl;
#endif
		saveRedundant();
#ifdef TOTAL_LOG
		cout << "Redundant worlds are saved!" << endl;
#endif
	}
	AWorld ret;
	name = getStrUpper(name);
	if (name.length() < 1) throw 1; // too short name
	for (char c : name) {
		if ((c<'A' || c>'Z') && (c<'0' || c>'9'))
			throw 2; // wrong name
	}
	if (name == "EXIT") {
		throw 3;
	}
	if (name == "CON" || name == "PRN" || name == "AUX" || name == "NUL" || name == "COM1" || name == "COM2" || name == "COM3" || name == "COM4" || name == "COM5" || name == "COM6" || name == "COM7" || name == "COM8" || name == "COM9" || name == "LPT1" || name == "LPT2" || name == "LPT3" || name == "LPT4" || name == "LPT5" || name == "LPT6" || name == "LPT7" || name == "LPT8" || name == "LPT9") throw 3;
	for (int i = 0; i < worlds.size(); i++) {
		if (worlds.at(i).name == name)
		{
			ret.id = i;
			ret.info = worlds.at(i);
			ret.ptr = &worlds.at(i);
			return ret;
		}

	}
	std::ifstream ifs("worlds/" + name + ".json");
	if (ifs.is_open()) {

		json j;
		ifs >> j;
		WorldInfo info;
		info.name = j["name"].get<string>();
		info.width = j["width"];
		info.height = j["height"];
		info.owner = j["owner"].get<string>();
		info.isPublic = j["isPublic"];
		
		info.weather = j["weather"];
		json tiles = j["tiles"];
		int square = info.width*info.height;
		info.items = new WorldItem[square];
		for (int i = 0; i < square; i++) {
			info.items[i].foreground = tiles[i]["fg"];
			info.items[i].background = tiles[i]["bg"];
		}
		worlds.push_back(info);
		ret.id = worlds.size() - 1;
		ret.info = info;
		ret.ptr = &worlds.at(worlds.size() - 1);
		return ret;
	}
	else {
		WorldInfo info = generateWorld(name, 100, 60);
		

		worlds.push_back(info);
		ret.id = worlds.size() - 1;
		ret.info = info;
		ret.ptr = &worlds.at(worlds.size() - 1);
		return ret;
	}
	throw 1;
}

WorldInfo WorldDB::get(string name) {

	return this->get2(name).info;
}

void WorldDB::flush(WorldInfo info)
{
	std::ofstream o("worlds/" + info.name + ".json");
	if (!o.is_open()) {
		cout << GetLastError() << endl;
	}
	json j;
	j["name"] = info.name;
	j["width"] = info.width;
	j["height"] = info.height;
	j["owner"] = info.owner;
	j["isPublic"] = info.isPublic;
	
	j["weather"] = info.weather;
	json tiles = json::array();
	int square = info.width*info.height;

	for (int i = 0; i < square; i++)
	{
		json tile;
		tile["fg"] = info.items[i].foreground;
		tile["bg"] = info.items[i].background;
		tiles.push_back(tile);
	}
	j["tiles"] = tiles;
	o << j << std::endl;
}

void WorldDB::flush2(AWorld info)
{
	this->flush(info.info);
}

void WorldDB::save(AWorld info)
{
	flush2(info);
	delete info.info.items;
	worlds.erase(worlds.begin() + info.id);
}


void WorldDB::saveAll()
{
	for (int i = 0; i < worlds.size(); i++) {
		flush(worlds.at(i));
		delete worlds.at(i).items;
	}
	worlds.clear();
}

vector<WorldInfo> WorldDB::getRandomWorlds() {
	vector<WorldInfo> ret;
	for (int i = 0; i < ((worlds.size() < 10) ? worlds.size() : 10); i++)
	{ // load first four worlds, it is excepted that they are special
		ret.push_back(worlds.at(i));
	}
	// and lets get up to 6 random
	if (worlds.size() > 4) {
		for (int j = 0; j < 6; j++)
		{
			bool isPossible = true;
			WorldInfo world = worlds.at(rand() % (worlds.size() - 4));
			for (int i = 0; i < ret.size(); i++)
			{
				if (world.name == ret.at(i).name || world.name == "EXIT")
				{
					isPossible = false;
				}
			}
			if (isPossible)
				ret.push_back(world);
		}
	}
	return ret;
}

void WorldDB::saveRedundant()
{
	for (int i = 4; i < worlds.size(); i++) {
		bool canBeFree = true;
		ENetPeer * currentPeer;

		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (((PlayerInfo*)(currentPeer->data))->currentWorld == worlds.at(i).name)
				canBeFree = false;
		}
		if (canBeFree)
		{
			flush(worlds.at(i));
			delete worlds.at(i).items;
			worlds.erase(worlds.begin() + i);
			i--;
		}
	}
}

//WorldInfo world;
//vector<WorldInfo> worlds;
WorldDB worldDB;

void saveAllWorlds() // atexit hack plz fix
{
	cout << "Saving worlds..." << endl;
	enet_host_destroy(server);
	worldDB.saveAll();
	cout << "Worlds saved!" << endl;
}

WorldInfo* getPlyersWorld(ENetPeer* peer)
{
	try {
		return worldDB.get2(((PlayerInfo*)(peer->data))->currentWorld).ptr;
	} catch(int e) {
		return NULL;
	}
}

struct PlayerMoving {
	int packetType;
	int netID;
	float x;
	float y;
	int characterState;
	int plantingTree;
	float XSpeed;
	float YSpeed;
	int punchX;
	int punchY;

};


enum ClothTypes {
	HAIR,
	SHIRT,
	PANTS,
	FEET,
	FACE,
	HAND,
	BACK,
	MASK,
	NECKLACE,
	ANCES,
	NONE
};

enum BlockTypes {
	FOREGROUND,
	BACKGROUND,
	SEED,
	PAIN_BLOCK,
	BEDROCK,
	MAIN_DOOR,
	SIGN,
	DOOR,
	CLOTHING,
	FIST,
	CONSUMMABLE,
	CHECKPOINT,
	GATEWAY,
	LOCK,
	WEATHER_MACHINE,
	JAMMER,
	GEM,
	BOARD,
	UNKNOWN
};


struct ItemDefinition {
	int id;

	unsigned char editableType = 0;
	unsigned char itemCategory = 0;
	unsigned char actionType = 0;
	unsigned char hitSoundType = 0;

	string name;

	string texture = "";
	int textureHash = 0;
	unsigned char itemKind = 0;
	int val1;
	unsigned char textureX = 0;
	unsigned char textureY = 0;
	unsigned char spreadType = 0;
	unsigned char isStripeyWallpaper = 0;
	unsigned char collisionType = 0;

	unsigned char breakHits = 0;

	int dropChance = 0;
	unsigned char clothingType = 0;
	BlockTypes blockType;
	int growTime;
	ClothTypes clothType;
	int16_t rarity;
	unsigned char maxAmount = 0;
	string extraFile = "";
	int extraFileHash = 0;
	int audioVolume = 0;
	string petName = "";
	string petPrefix = "";
	string petSuffix = "";
	string petAbility = "";
	unsigned	char seedBase = 0;
	unsigned	char seedOverlay = 0;
	unsigned	char treeBase = 0;
	unsigned	char treeLeaves = 0;
	int seedColor = 0;
	int seedOverlayColor = 0;
	bool isMultiFace = false;
	short val2;
	short isRayman = 0;
	string extraOptions = "";
	string texture2 = "";
	string extraOptions2 = "";
	string punchOptions = "";
	string description = "Nothing to see.";
};

vector<ItemDefinition> itemDefs;

ItemDefinition getItemDef(int id)
{
	if (id < itemDefs.size() && id > -1)
		return itemDefs.at(id);
	/*for (int i = 0; i < itemDefs.size(); i++)
	{
		if (id == itemDefs.at(i).id)
		{
			return itemDefs.at(i);
		}
	}*/
	throw 0;
	return itemDefs.at(0);
}

void craftItemDescriptions() {
	int current = -1;
	std::ifstream infile("Descriptions.txt");
	for (std::string line; getline(infile, line);)
	{
		if (line.length() > 3 && line[0] != '/' && line[1] != '/')
		{
			vector<string> ex = explode("|", line);
			ItemDefinition def;
			if (atoi(ex[0].c_str()) + 1 < itemDefs.size())
			{
				itemDefs.at(atoi(ex[0].c_str())).description = ex[1];
				if (!(atoi(ex[0].c_str()) % 2))
					itemDefs.at(atoi(ex[0].c_str()) + 1).description = "This is a tree.";
			}
		}
	}
}

std::ifstream::pos_type filesize(const char* filename)
{
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

uint32_t HashString(unsigned char* str, int len)
{
	if (!str) return 0;

	unsigned char* n = (unsigned char*)str;
	uint32_t acc = 0x55555555;

	if (len == 0)
	{
		while (*n)
			acc = (acc >> 27) + (acc << 5) + *n++;
	}
	else
	{
		for (int i = 0; i < len; i++)
		{
			acc = (acc >> 27) + (acc << 5) + *n++;
		}
	}
	return acc;

}

unsigned char* getA(string fileName, int* pSizeOut, bool bAddBasePath, bool bAutoDecompress)
{
	unsigned char* pData = NULL;
	FILE* fp = fopen(fileName.c_str(), "rb");
	if (!fp)
	{
		cout << "File not found" << endl;
		if (!fp) return NULL;
	}

	fseek(fp, 0, SEEK_END);
	*pSizeOut = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	pData = (unsigned char*)new unsigned char[((*pSizeOut) + 1)];
	if (!pData)
	{
		printf("Out of memory opening %s?", fileName.c_str());
		return 0;
	}
	pData[*pSizeOut] = 0;
	fread(pData, *pSizeOut, 1, fp);
	fclose(fp);

	return pData;
}

int itemdathash;
void buildItemsDatabase()
{
	string secret = "PBG892FXX982ABC*";
	std::ifstream file("items.dat", std::ios::binary | std::ios::ate);
	int size = file.tellg();
	itemsDatSize = size;
	char* data = new char[size];
	file.seekg(0, std::ios::beg);

	if (file.read((char*)(data), size))
	{
		itemsDat = new BYTE[60 + size];
		int MessageType = 0x4;
		int PacketType = 0x10;
		int NetID = -1;
		int CharState = 0x8;

		memset(itemsDat, 0, 60);
		memcpy(itemsDat, &MessageType, 4);
		memcpy(itemsDat + 4, &PacketType, 4);
		memcpy(itemsDat + 8, &NetID, 4);
		memcpy(itemsDat + 16, &CharState, 4);
		memcpy(itemsDat + 56, &size, 4);
		file.seekg(0, std::ios::beg);
		if (file.read((char*)(itemsDat + 60), size))
		{
			uint8_t* pData;
			int size = 0;
			const char filename[] = "items.dat";
			size = filesize(filename);
			pData = getA((string)filename, &size, false, false);
			cout << "Updating items data success! Hash: " << HashString((unsigned char*)pData, size) << endl;
			itemdathash = HashString((unsigned char*)pData, size);
			file.close();
		}
	}
	else {
		cout << "Updating items data failed!" << endl;
		exit(0);
	}
	int itemCount;
	int memPos = 0;
	int16_t itemsdatVersion = 0;
	memcpy(&itemsdatVersion, data + memPos, 2);
	memPos += 2;
	memcpy(&itemCount, data + memPos, 4);
	memPos += 4; 
	for (int i = 0; i < itemCount; i++) { 
		ItemDefinition tile; 

		{
			memcpy(&tile.id, data + memPos, 4);
			memPos += 4;
		}
		{
			tile.editableType = data[memPos];
			memPos += 1;
		}
		{
			tile.itemCategory = data[memPos];
			memPos += 1;
		}
		{
			tile.actionType = data[memPos];
			memPos += 1;
		}
		{
			tile.hitSoundType = data[memPos];
			memPos += 1;
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				tile.name += data[memPos] ^ (secret[(j + tile.id) % secret.length()]);

				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				tile.texture += data[memPos];
				memPos++;
			}
		}
		memcpy(&tile.textureHash, data + memPos, 4);
		memPos += 4;
		tile.itemKind = memPos[data];
		memPos += 1;
		memcpy(&tile.val1, data + memPos, 4);
		memPos += 4;
		tile.textureX = data[memPos];
		memPos += 1;
		tile.textureY = data[memPos];
		memPos += 1;
		tile.spreadType = data[memPos];
		memPos += 1;
		tile.isStripeyWallpaper = data[memPos];
		memPos += 1;
		tile.collisionType = data[memPos];
		memPos += 1;
		tile.breakHits = data[memPos] / 6;
		memPos += 1;
		memcpy(&tile.dropChance, data + memPos, 4);
		memPos += 4;
		tile.clothingType = data[memPos];
		memPos += 1;
		memcpy(&tile.rarity, data + memPos, 2);
		memPos += 2;
		tile.maxAmount = data[memPos];
		memPos += 1;
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				tile.extraFile += data[memPos];
				memPos++;
			}
		}
		memcpy(&tile.extraFileHash, data + memPos, 4);
		memPos += 4;
		memcpy(&tile.audioVolume, data + memPos, 4);
		memPos += 4;
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				tile.petName += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				tile.petPrefix += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				tile.petSuffix += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				tile.petAbility += data[memPos];
				memPos++;
			}
		}
		{
			tile.seedBase = data[memPos];
			memPos += 1;
		}
		{
			tile.seedOverlay = data[memPos];
			memPos += 1;
		}
		{
			tile.treeBase = data[memPos];
			memPos += 1;
		}
		{
			tile.treeLeaves = data[memPos];
			memPos += 1;
		}
		{
			memcpy(&tile.seedColor, data + memPos, 4);
			memPos += 4;
		}
		{
			memcpy(&tile.seedOverlayColor, data + memPos, 4);
			memPos += 4;
		}
		memPos += 4; // deleted ingredients
		{
			memcpy(&tile.growTime, data + memPos, 4);
			memPos += 4;
		}
		memcpy(&tile.val2, data + memPos, 2);
		memPos += 2;
		memcpy(&tile.isRayman, data + memPos, 2);
		memPos += 2;
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				tile.extraOptions += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				tile.texture2 += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				tile.extraOptions2 += data[memPos];
				memPos++;
			}
		}
		memPos += 80;
		if (itemsdatVersion >= 12) {
			{
				int16_t strLen = *(int16_t*)&data[memPos];
				memPos += 2;
				for (int j = 0; j < strLen; j++) {
					tile.punchOptions += data[memPos];
					memPos++;
				}
			}
		}
		memPos += 13;
		memPos += 4;
		if (i != tile.id)
			cout << "Item are unordered!" << i << "/" << tile.id << endl;

		switch (tile.actionType) {
		case 0:
			tile.blockType = BlockTypes::FIST;
			break;
		case 1:
			// wrench tool
			break;
		case 2:
			tile.blockType = BlockTypes::DOOR;
			break;
		case 3:
			tile.blockType = BlockTypes::LOCK;
			break;
		case 4:
			tile.blockType = BlockTypes::GEM;
			break;
		case 8:
			tile.blockType = BlockTypes::CONSUMMABLE;
			break;
		case 9:
			tile.blockType = BlockTypes::GATEWAY;
			break;
		case 10:
			tile.blockType = BlockTypes::SIGN;
			break;
		case 13:
			tile.blockType = BlockTypes::MAIN_DOOR;
			break;
		case 15:
			tile.blockType = BlockTypes::BEDROCK;
			break;
		case 17:
			tile.blockType = BlockTypes::FOREGROUND;
			break;
		case 18:
			tile.blockType = BlockTypes::BACKGROUND;
			break;
		case 19:
			tile.blockType = BlockTypes::SEED;
			break;
		case 20:
			tile.blockType = BlockTypes::CLOTHING; 
				switch(tile.clothingType){
					case 0: tile.clothType = ClothTypes::HAIR;
						break;
					case 1: tile.clothType = ClothTypes::SHIRT;
						break;
					case 2: tile.clothType = ClothTypes::PANTS;
						break;
					case 3: tile.clothType = ClothTypes::FEET;
						break; 
					case 4: tile.clothType = ClothTypes::FACE;
						break;
					case 5: tile.clothType = ClothTypes::HAND;
						break;
					case 6: tile.clothType = ClothTypes::BACK;
						break;
					case 7: tile.clothType = ClothTypes::MASK;
						break;
					case 8: tile.clothType = ClothTypes::NECKLACE;
						break;
						
				} 

			break;
		case 26: // portal
			tile.blockType = BlockTypes::DOOR;
			break;
		case 27:
			tile.blockType = BlockTypes::CHECKPOINT;
			break;
		case 28: // piano note
			tile.blockType = BlockTypes::BACKGROUND;
			break;
		case 41:
			tile.blockType = BlockTypes::WEATHER_MACHINE;
			break;
		case 34: // bulletin boardd
			tile.blockType = BlockTypes::BOARD;
			break;
		case 107: // ances
			tile.blockType = BlockTypes::CLOTHING;
			tile.clothType = ClothTypes::ANCES;
			break;
		default:
			 break;

		}
 

		// -----------------
		itemDefs.push_back(tile);
	} 
	craftItemDescriptions();
}

void addAdmin(string username, string password, int level)
{
	Admin admin;
	admin.username = username;
	admin.password = password;
	admin.level = level;
	admins.push_back(admin);
	time_t now = time(0);
	tm *ltm = localtime(&now);
	cout << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "]" << " System added " + username + " as a developer/moderator with level " + std::to_string(level) + "\n";
}

int getAdminLevel(string username, string password) {
	for (int i = 0; i < admins.size(); i++) {
		Admin admin = admins[i];
		if (admin.username == username && admin.password == password) {
			return admin.level;
		}
	}
	return 0;
}

bool canSB(string username, string password) {
	bool haveSuperSupporterName = true;
	for (int i = 0; i < admins.size(); i++) {
		Admin admin = admins[i];
		if (admin.username == username && admin.password == password && admin.level>1) {
			using namespace std::chrono;
			if (admin.lastSB + 900000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count() || admin.level == 999)
			{
				admins[i].lastSB = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
				return true;
			}
			else {
				return false;
			}
		}
	}
	return false;
}

bool canClear(string username, string password) {
	bool haveSuperSupporterName = true;
	for (int i = 0; i < admins.size(); i++) {
		Admin admin = admins[i];
		if (admin.username == username && admin.password == password) {
			return admin.level > 0;
		}
	}
	return false;
}

bool isSuperAdmin(string username, string password) {
	bool haveSuperSupporterName = true;
	for (int i = 0; i < admins.size(); i++) {
		Admin admin = admins[i];
		bool haveSuperSupporterName = true;
		if (admin.username == username && admin.password == password && admin.level == 999) {
			return true;
		}
	}
	return false;
}

bool isModerator(string username, string password) {
	bool haveSuperSupporterName = true;
	for (int i = 0; i < admins.size(); i++) {
		Admin admin = admins[i];
		bool haveSuperSupporterName = true;
		if (admin.username == username && admin.password == password && admin.level == 500) {
			bool haveSuperSupporterName = true;
			return true;
		}
	}
	
	return false;
}


bool isHere(ENetPeer* peer, ENetPeer* peer2)
{
	return ((PlayerInfo*)(peer->data))->currentWorld == ((PlayerInfo*)(peer2->data))->currentWorld;
}

void sendInventory(ENetPeer* peer, PlayerInventory inventory)
{
	int inventoryLen = inventory.items.size();
	int packetLen = 66 + (inventoryLen * 4) + 4;
	BYTE* data2 = new BYTE[packetLen];
	int MessageType = 0x4;
	int PacketType = 0x9;
	int NetID = -1;
	int CharState = 0x8;

	memset(data2, 0, packetLen);
	memcpy(data2, &MessageType, 4);
	memcpy(data2 + 4, &PacketType, 4);
	memcpy(data2 + 8, &NetID, 4);
	memcpy(data2 + 16, &CharState, 4);
	int endianInvVal = _byteswap_ulong(inventoryLen);
	memcpy(data2 + 66 - 4, &endianInvVal, 4);
	endianInvVal = _byteswap_ulong(inventory.inventorySize);
	memcpy(data2 + 66 - 8, &endianInvVal, 4);
	int val = 0;
	for (int i = 0; i < inventoryLen; i++)
	{
		val = 0;
		val |= inventory.items.at(i).itemID;
		val |= inventory.items.at(i).itemCount << 16;
		val &= 0x00FFFFFF;
		val |= 0x00 << 24;
		memcpy(data2 + (i * 4) + 66, &val, 4);
	}
	ENetPacket* packet3 = enet_packet_create(data2,
		packetLen,
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet3);
	delete data2;
}

BYTE* packPlayerMoving(PlayerMoving* dataStruct)
{
	BYTE* data = new BYTE[56];
	for (int i = 0; i < 56; i++)
	{
		data[i] = 0;
	}
	memcpy(data, &dataStruct->packetType, 4);
	memcpy(data + 4, &dataStruct->netID, 4);
	memcpy(data + 12, &dataStruct->characterState, 4);
	memcpy(data + 20, &dataStruct->plantingTree, 4);
	memcpy(data + 24, &dataStruct->x, 4);
	memcpy(data + 28, &dataStruct->y, 4);
	memcpy(data + 32, &dataStruct->XSpeed, 4);
	memcpy(data + 36, &dataStruct->YSpeed, 4);
	memcpy(data + 44, &dataStruct->punchX, 4);
	memcpy(data + 48, &dataStruct->punchY, 4);
	return data;
}

PlayerMoving* unpackPlayerMoving(BYTE* data)
{
	PlayerMoving* dataStruct = new PlayerMoving;
	memcpy(&dataStruct->packetType, data, 4);
	memcpy(&dataStruct->netID, data + 4, 4);
	memcpy(&dataStruct->characterState, data + 12, 4);
	memcpy(&dataStruct->plantingTree, data + 20, 4);
	memcpy(&dataStruct->x, data + 24, 4);
	memcpy(&dataStruct->y, data + 28, 4);
	memcpy(&dataStruct->XSpeed, data + 32, 4);
	memcpy(&dataStruct->YSpeed, data + 36, 4);
	memcpy(&dataStruct->punchX, data + 44, 4);
	memcpy(&dataStruct->punchY, data + 48, 4);
	return dataStruct;
}

void SendPacket(int a1, string a2, ENetPeer* enetPeer)
{
	if (enetPeer)
	{
		ENetPacket* v3 = enet_packet_create(0, a2.length() + 5, 1);
		memcpy(v3->data, &a1, 4);
		//*(v3->data) = (DWORD)a1;
		memcpy((v3->data) + 4, a2.c_str(), a2.length());

		//cout << std::hex << (int)(char)v3->data[3] << endl;
		enet_peer_send(enetPeer, 0, v3);
	}
}

void SendPacketRaw(int a1, void *packetData, size_t packetDataSize, void *a4, ENetPeer* peer, int packetFlag)
{
	ENetPacket *p;

	if (peer) // check if we have it setup
	{
		if (a1 == 4 && *((BYTE *)packetData + 12) & 8)
		{
			p = enet_packet_create(0, packetDataSize + *((DWORD *)packetData + 13) + 5, packetFlag);
			int four = 4;
			memcpy(p->data, &four, 4);
			memcpy((char *)p->data + 4, packetData, packetDataSize);
			memcpy((char *)p->data + packetDataSize + 4, a4, *((DWORD *)packetData + 13));
			enet_peer_send(peer, 0, p);
		}
		else
		{
			p = enet_packet_create(0, packetDataSize + 5, packetFlag);
			memcpy(p->data, &a1, 4);
			memcpy((char *)p->data + 4, packetData, packetDataSize);
			enet_peer_send(peer, 0, p);
		}
	}
	delete (char*)packetData;
}
void sendClothes(ENetPeer* peer)
{
	ENetPeer * currentPeer;
	gamepacket_t p(0, ((PlayerInfo*)(peer->data))->netID);
	p.Insert("OnSetClothing");
	p.Insert(((PlayerInfo*)(peer->data))->cloth_hair, ((PlayerInfo*)(peer->data))->cloth_shirt, ((PlayerInfo*)(peer->data))->cloth_pants);
	p.Insert(((PlayerInfo*)(peer->data))->cloth_feet, ((PlayerInfo*)(peer->data))->cloth_face, ((PlayerInfo*)(peer->data))->cloth_hand);
	p.Insert(((PlayerInfo*)(peer->data))->cloth_back, ((PlayerInfo*)(peer->data))->cloth_mask, ((PlayerInfo*)(peer->data))->cloth_necklace);
	p.Insert(((PlayerInfo*)(peer->data))->skinColor);
	p.Insert(((PlayerInfo*)(peer->data))->cloth_ances, 0.0f, 0.0f);
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer))
		{
			p.CreatePacket(currentPeer);
		}

	}
}
void sendState(ENetPeer* peer) {
	//return; // TODO
	PlayerInfo* info = ((PlayerInfo*)(peer->data));
	int netID = info->netID;
	ENetPeer * currentPeer;
	int state = getState(info);
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer)) {
			PlayerMoving data;
			data.packetType = 0x14;
			data.characterState = 0; // animation
			data.x = 1000;
			data.y = 100;
			data.punchX = 0;
			data.punchY = 0;
			data.XSpeed = 300;
			data.YSpeed = 600;
			data.netID = netID;
			data.plantingTree = state;
			BYTE* raw = packPlayerMoving(&data);
			int var = 0x808000; // placing and breking
			memcpy(raw + 1, &var, 3);
			float waterspeed = 125.0f;
			memcpy(raw + 16, &waterspeed, 4);
			SendPacketRaw(4, raw, 56, 0, currentPeer, ENET_PACKET_FLAG_RELIABLE);
		}
	}
	// TODO
}

	void onPeerConnect(ENetPeer* peer)
	{
		ENetPeer * currentPeer;

		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (peer != currentPeer)
			{
				if (isHere(peer, currentPeer))
				{
					string netIdS = std::to_string(((PlayerInfo*)(currentPeer->data))->netID);
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 499 || ((PlayerInfo*)(peer->data))->adminLevel > 499) {
						if (((PlayerInfo*)(peer->data))->isInvisible == true) {
							packet::onspawn(peer, "spawn|avatar\nnetID|" + netIdS + "\nuserID|" + netIdS + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(((PlayerInfo*)(currentPeer->data))->x) + "|" + std::to_string(((PlayerInfo*)(currentPeer->data))->y) + "\nname|``" + ((PlayerInfo*)(currentPeer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(currentPeer->data))->country + "\ninvis|1\nmstate|1\nsmstate|1\n"); // ((PlayerInfo*)(server->peers[i].data))->tankIDName
						}
						else {
							packet::onspawn(peer, "spawn|avatar\nnetID|" + netIdS + "\nuserID|" + netIdS + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(((PlayerInfo*)(currentPeer->data))->x) + "|" + std::to_string(((PlayerInfo*)(currentPeer->data))->y) + "\nname|``" + ((PlayerInfo*)(currentPeer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(currentPeer->data))->country + "\ninvis|0\nmstate|1\nsmstate|1\n"); // ((PlayerInfo*)(server->peers[i].data))->tankIDName
						}
					}
					else {
						packet::onspawn(peer, "spawn|avatar\nnetID|" + netIdS + "\nuserID|" + netIdS + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(((PlayerInfo*)(currentPeer->data))->x) + "|" + std::to_string(((PlayerInfo*)(currentPeer->data))->y) + "\nname|``" + ((PlayerInfo*)(currentPeer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(currentPeer->data))->country + "\ninvis|0\nmstate|0\nsmstate|0\n"); // ((PlayerInfo*)(server->peers[i].data))->tankIDName
					}
					string netIdS2 = std::to_string(((PlayerInfo*)(peer->data))->netID);
					if (getAdminLevel(((PlayerInfo*)(currentPeer->data))->rawName, ((PlayerInfo*)(currentPeer->data))->tankIDPass) > 499 || ((PlayerInfo*)(currentPeer->data))->adminLevel > 499) {
						packet::onspawn(currentPeer, "spawn|avatar\nnetID|" + netIdS2 + "\nuserID|" + netIdS2 + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(((PlayerInfo*)(peer->data))->x) + "|" + std::to_string(((PlayerInfo*)(peer->data))->y) + "\nname|``" + ((PlayerInfo*)(peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(peer->data))->country + "\ninvis|0\nmstate|1\nsmstate|1\n"); // ((PlayerInfo*)(server->peers[i].data))->tankIDName
						if (((PlayerInfo*)(peer->data))->isInvisible == true) {
							packet::onspawn(currentPeer, "spawn|avatar\nnetID|" + netIdS2 + "\nuserID|" + netIdS2 + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(((PlayerInfo*)(peer->data))->x) + "|" + std::to_string(((PlayerInfo*)(peer->data))->y) + "\nname|``" + ((PlayerInfo*)(peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(peer->data))->country + "\ninvis|1\nmstate|1\nsmstate|1\n"); // ((PlayerInfo*)(server->peers[i].data))->tankIDName
						}
						else {
							packet::onspawn(currentPeer, "spawn|avatar\nnetID|" + netIdS2 + "\nuserID|" + netIdS2 + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(((PlayerInfo*)(peer->data))->x) + "|" + std::to_string(((PlayerInfo*)(peer->data))->y) + "\nname|``" + ((PlayerInfo*)(peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(peer->data))->country + "\ninvis|0\nmstate|1\nsmstate|1\n"); // ((PlayerInfo*)(server->peers[i].data))->tankIDName

						}
					}
					else {
						packet::onspawn(currentPeer, "spawn|avatar\nnetID|" + netIdS2 + "\nuserID|" + netIdS2 + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(((PlayerInfo*)(peer->data))->x) + "|" + std::to_string(((PlayerInfo*)(peer->data))->y) + "\nname|``" + ((PlayerInfo*)(peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(peer->data))->country + "\ninvis|0\nmstate|0\nsmstate|0\n"); // ((PlayerInfo*)(server->peers[i].data))->tankIDName
					}
					sendState(peer);
					sendState(currentPeer);
					sendClothes(peer);
					sendClothes(currentPeer);
				}
			}
		}
		
	}
	void sendPuncheffectpeer(ENetPeer* peer, int punch) {
		//return; // TODO
		PlayerInfo* info = ((PlayerInfo*)(peer->data));
		int netID = info->netID;
		//ENetPeer * currentPeer;
		int state = getState(info);

		PlayerMoving data;
		data.packetType = 0x14;
		data.characterState = ((PlayerInfo*)(peer->data))->characterState; // animation
		data.x = 1000;
		data.y = 100;
		data.punchX = -1;
		data.punchY = -1;
		data.XSpeed = 300;
		data.YSpeed = 600;
		data.netID = netID;
		data.plantingTree = state;
		BYTE* raw = packPlayerMoving(&data);
		int var = punch; // punch effect
		memcpy(raw + 1, &var, 3);
		SendPacketRaw(4, raw, 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);


		// TODO 
	}
	void updateAllClothes(ENetPeer* peer)
	{
		ENetPeer * currentPeer;
		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (isHere(peer, currentPeer))
			{
				gamepacket_t p(0, ((PlayerInfo*)(peer->data))->netID);
				p.Insert("OnSetClothing");
				p.Insert(((PlayerInfo*)(peer->data))->cloth_hair, ((PlayerInfo*)(peer->data))->cloth_shirt, ((PlayerInfo*)(peer->data))->cloth_pants);
				p.Insert(((PlayerInfo*)(peer->data))->cloth_feet, ((PlayerInfo*)(peer->data))->cloth_face, ((PlayerInfo*)(peer->data))->cloth_hand);
				p.Insert(((PlayerInfo*)(peer->data))->cloth_back, ((PlayerInfo*)(peer->data))->cloth_mask, ((PlayerInfo*)(peer->data))->cloth_necklace);
				p.Insert(((PlayerInfo*)(peer->data))->skinColor);
				p.Insert(((PlayerInfo*)(peer->data))->cloth_ances, 0.0f, 0.0f);
				p.CreatePacket(currentPeer);

				gamepacket_t p2(0, ((PlayerInfo*)(peer->data))->netID);
				p2.Insert("OnSetClothing");
				p2.Insert(((PlayerInfo*)(peer->data))->cloth_hair, ((PlayerInfo*)(peer->data))->cloth_shirt, ((PlayerInfo*)(peer->data))->cloth_pants);
				p2.Insert(((PlayerInfo*)(peer->data))->cloth_feet, ((PlayerInfo*)(peer->data))->cloth_face, ((PlayerInfo*)(peer->data))->cloth_hand);
				p2.Insert(((PlayerInfo*)(peer->data))->cloth_back, ((PlayerInfo*)(peer->data))->cloth_mask, ((PlayerInfo*)(peer->data))->cloth_necklace);
				p2.Insert(((PlayerInfo*)(peer->data))->skinColor);
				p2.Insert(((PlayerInfo*)(peer->data))->cloth_ances, 0.0f, 0.0f);
				p2.CreatePacket(peer);
			}
		}
	}

	
	
	void sendPData(ENetPeer* peer, PlayerMoving* data)
	{
		ENetPeer * currentPeer;

		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (peer != currentPeer)
			{
				if (isHere(peer, currentPeer))
				{
					data->netID = ((PlayerInfo*)(peer->data))->netID;

					SendPacketRaw(4, packPlayerMoving(data), 56, 0, currentPeer, ENET_PACKET_FLAG_RELIABLE);
				}
			}
		}
	}

	int getPlayersCountInWorld(string name)
	{
		int count = 0;
		ENetPeer * currentPeer;
		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (((PlayerInfo*)(currentPeer->data))->currentWorld == name)
				count++;
			count - 1;//removing current player that wants to call it
		}
		return count;
	}

	void sendRoulete(ENetPeer* peer, int x, int y)
	{
		ENetPeer* currentPeer;
		int val = rand() % 37;
		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (isHere(peer, currentPeer))
			{
				gamepacket_t p(2500);
				p.Insert("OnTalkBubble");
				p.Insert(((PlayerInfo*)(peer->data))->netID);
				p.Insert("`w[" + ((PlayerInfo*)(peer->data))->displayName + " `wspun the wheel and got `6" + std::to_string(val) + "`w!]");
				p.Insert(0);
				p.CreatePacket(currentPeer);
			}
		}
	}


	void sendNothingHappened(ENetPeer* peer, int x, int y) {
		PlayerMoving data;
		data.netID = ((PlayerInfo*)(peer->data))->netID;
		data.packetType = 0x8;
		data.plantingTree = 0;
		data.netID = -1;
		data.x = x;
		data.y = y;
		data.punchX = x;
		data.punchY = y;
		SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
	}
	void sendRing(ENetPeer* peer) {
		gamepacket_t ring;
		ring.Insert("OnDialogRequest");
		ring.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`9 The Ringmaster|left|1900|\n\nadd_spacer|small|\nadd_textbox|`oCome one, come all, to the most extraordinary show in Growtopia! I am the ringmaster. That means I know a lot about Rings! For 10 Golden Tickets, I might even tell you how you can get a Ring of your own...|\nadd_spacer|small|\nadd_button|givecarnitickets|`9Give 10 Golden Tickets|noflags|0|0||\nadd_spacer|small|\nadd_textbox|`oIf you had 10 rings of the same type maybe we could have made a deal...|left|\nadd_spacer|small|\nadd_quick_exit|\n\nend_dialog|gazette|Darn it!|\n");//|noflags|0|0|
		ring.CreatePacket(peer);
    }
	void sendWizard(ENetPeer* peer) {
		gamepacket_t p;
		p.Insert("OnDialogRequest");
		p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`9The Legendary Wizard``|left|1790|\nadd_textbox|`oGreetings, Traveler! I am the Legendary Wizard. Should to embark on a Legendary Quest, Simply choose one below.|left|\nadd_spacer|small|\nadd_button|ltitle|Quest for Honor|noflags|0|0|\nadd_button|lsky|Quest Of The Sky|noflags|0|0|\nadd_button|ldrag|Quest for Fire|noflags|0|0|\nadd_button|lbot|Quest Of Steel|noflags|0|0|\nadd_button|lwings|Quest Of The Heavens|noflags|0|0|\nadd_button|lkat|Quest of Blade|noflags|0|0|\nadd_button|lwhip|Quest for Condour|noflags|0|0|\nadd_spacer|small|\nadd_button|c0co|Close|noflags|0|0|\nadd_quick_exit|\nnend_dialog|gazette||OK|");
		p.CreatePacket(peer);

	}
	void sendSaveSet(ENetPeer * peer) {
		ENetPeer * currentPeer;
		PlayerInfo* p = ((PlayerInfo*)(peer->data));
		string name = ((PlayerInfo*)(peer->data))->rawName;
		std::ifstream od("players/" + name + ".json");
		if (od.is_open()) {
		}

		std::ofstream o("players/" + name + ".json");
		if (!o.is_open()) {
			cout << GetLastError() << endl;
			_getch();
		}
		json j;
		j["isSuspended"] = p->isSuspended;
		j["username"] = p->rawName;
		j["password"] = p->pactPass;
		j["email"] = p->tankIDEmail;
		j["adminLevel"] = p->adminLevel;
		j["playerWarnings"] = p->playerWarnings;
		j["ClothBack"] = p->cloth_back;
		j["ClothHand"] = p->cloth_hand;
		j["ClothFace"] = p->cloth_face;
		j["ClothShirt"] = p->cloth_shirt;
		j["ClothPants"] = p->cloth_pants;
		j["ClothNeck"] = p->cloth_necklace;
		j["ClothHair"] = p->cloth_hair;
		j["ClothFeet"] = p->cloth_feet;
		j["ClothMask"] = p->cloth_mask;
		j["hasLegenName"] = p->hasLegenName;
		j["hasLegenDrag"] = p->hasLegenDrag;
		j["hasLegenBot"] = p->hasLegenBot;
		j["hasLegenWing"] = p->hasLegenWing;
		j["hasLegenKatana"] = p->hasLegenKatana;
		j["hasLegenWhip"] = p->hasLegenWhip;
		j["hasLegenKnight"] = p->hasLegenKnight;
		j["gems"] = p->gems;
		j["level"] = p->level;
		j["xp"] = p->xp;
		o << j << std::endl;
		//packet::consolemessage(peer, "`oSet has been successfully saved!");
	}
	void sendSaveInventory(ENetPeer * peer) {
		ENetPeer * currentPeer;
		PlayerInfo* p = ((PlayerInfo*)(peer->data));
		string name = ((PlayerInfo*)(peer->data))->rawName;
		std::ifstream od("inven/" + name + ".json");
		if (od.is_open()) {
		}

		std::ofstream o("inven/" + name + ".json");
		if (!o.is_open()) {
			cout << GetLastError() << endl;
			_getch();
		}
		json j;
		PlayerInventory inventory = ((PlayerInfo*)(peer->data))->inventory;
		int isize = ((PlayerInfo*)(peer->data))->inventory.items.size();
		j["isize"] = isize;
		json inven = json::array();
		for (int a = 0; a < isize; a++)
		{
			json inv;
			inv["id"] = ((PlayerInfo*)(peer->data))->inventory.items[a].itemID;
			inv["count"] = ((PlayerInfo*)(peer->data))->inventory.items[a].itemCount;
			inven.push_back(inv);
		}
		j["inv"] = inven;
		o << j << std::endl;

	}
	void sendWeatheral(ENetPeer * peer, WorldInfo* worldInfo) {
		
		gamepacket_t p;
		p.Insert("OnSetCurrentWeather");
		p.Insert(worldInfo->weather);
		p.CreatePacket(peer);
	}
	void sendEntered(ENetPeer * currentPeer, ENetPeer * peer) {
		
		
		int counts = 0;
		counts = -1;
		gamepacket_t enter1;
		enter1.Insert("OnTalkBubble");
		enter1.Insert(((PlayerInfo*)(peer->data))->netID);
		enter1.Insert("`5<`w" + ((PlayerInfo*)(peer->data))->displayName + "`5 entered, `w" + std::to_string(counts) + "`5 others here>``");
		enter1.Insert(0);
		//enter1.CreatePacket(peer);
		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (isHere(peer, currentPeer))
			{
				enter1.CreatePacket(currentPeer);
				sendState(currentPeer);
				sendClothes(currentPeer);

			}
		}
 }
void loadnews() {
	std::ifstream ifs("news.txt");
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));

	string target = "\r";
	string news = "";
	int found = -1;
	do {
		found = content.find(target, found + 1);
		if (found != -1) {
			news = content.substr(0, found) + content.substr(found + target.length());
		}
		else {
			news = content;
		}
	} while (found != -1);
	if(news != "") {
		
		newslist = news;
		cout << "loaded news ?? \n";
	}
}
void sendPlayerLeave(ENetPeer* peer, PlayerInfo* player)
{
	ENetPeer * currentPeer;
	int count = 0;
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		count++;
		count - 1;//current player online
	}
	gamepacket_t p;
	p.Insert("OnRemove");
	p.Insert("netID|" + std::to_string(player->netID) + "\n");
	gamepacket_t p2;
	p2.Insert("OnConsoleMessage");
	p2.Insert("`5<`w" + player->displayName + "`5 left, `w" + std::to_string(getPlayersCountInWorld(player->currentWorld) - 1) + "`5 others here>``");
	gamepacket_t p3;
	p3.Insert("OnConsoleMessage");
	p3.Insert("`oWhere would you like to go? (`w" + std::to_string(count) + "`o online)");
	sendSaveSet(peer);
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer)) {
			{
				p.CreatePacket(peer);

				{
					p.CreatePacket(currentPeer);
				}

			}
			{
				p2.CreatePacket(currentPeer);
				p3.CreatePacket(peer);
			}
		}
	}
}

void sendDrop(ENetPeer* peer, int netID, int x, int y, int item, int count, BYTE specialEffect)
{
	if (item >= 7068) return;
	if (item < 0) return;
	ENetPeer * currentPeer;
	string name = "";
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer)) {
			PlayerMoving data;
			data.packetType = 14;
			data.x = x;
			data.y = y;
			data.netID = netID;
			data.plantingTree = item;
			float val = count; // item count
			BYTE val2 = specialEffect;

			BYTE* raw = packPlayerMoving(&data);
			memcpy(raw + 16, &val, 4);
			memcpy(raw + 1, &val2, 1);

			SendPacketRaw(4, raw, 56, 0, currentPeer, ENET_PACKET_FLAG_RELIABLE);
		}
	}
}
void sendWorld(ENetPeer* peer, WorldInfo* worldInfo)
{
#ifdef TOTAL_LOG
	cout << "Entering a world..." << endl;
#endif

	((PlayerInfo*)(peer->data))->joinClothesUpdated = false;


	string worldName = worldInfo->name;

	int xSize = worldInfo->width;
	int ySize = worldInfo->height;
	int square = xSize * ySize;
	__int16 namelen = worldName.length();

	int alloc = (8 * square);
	int total = 78 + namelen + square + 24 + alloc;

	BYTE* data = new BYTE[total];
	int s1 = 4, s3 = 8, zero = 0;

	memset(data, 0, total);

	memcpy(data, &s1, 1);
	memcpy(data + 4, &s1, 1);
	memcpy(data + 16, &s3, 1);
	memcpy(data + 66, &namelen, 1);
	memcpy(data + 68, worldName.c_str(), namelen);
	memcpy(data + 68 + namelen, &xSize, 1);
	memcpy(data + 72 + namelen, &ySize, 1);
	memcpy(data + 76 + namelen, &square, 2);
	BYTE* blc = data + 80 + namelen;
	for (int i = 0; i < square; i++) {
		//removed cus some of blocks require tile extra and it will crash the world without
		memcpy(blc, &zero, 2);

		memcpy(blc + 2, &worldInfo->items[i].background, 2);
		int type = 0x00000000;
		// type 1 = locked
		if (worldInfo->items[i].water)
			type |= 0x04000000;
		if (worldInfo->items[i].glue)
			type |= 0x08000000;
		if (worldInfo->items[i].fire)
			type |= 0x10000000;
		if (worldInfo->items[i].red)
			type |= 0x20000000;
		if (worldInfo->items[i].green)
			type |= 0x40000000;
		if (worldInfo->items[i].blue)
			type |= 0x80000000;

		// int type = 0x04000000; = water
		// int type = 0x08000000 = glue
		// int type = 0x10000000; = fire
		// int type = 0x20000000; = red color
		// int type = 0x40000000; = green color
		// int type = 0x80000000; = blue color
		memcpy(blc + 4, &type, 4);
		blc += 8;
	}

	//int totalitemdrop = worldInfo->dropobject.size();
		//memcpy(blc, &totalitemdrop, 2);

	ENetPacket* packetw = enet_packet_create(data, total, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packetw);


	for (int i = 0; i < square; i++) {
		PlayerMoving data;
		//data.packetType = 0x14;
		data.packetType = 0x3;

		//data.characterState = 0x924; // animation
		data.characterState = 0x0; // animation
		data.x = i % worldInfo->width;
		data.y = i / worldInfo->height;
		data.punchX = i % worldInfo->width;
		data.punchY = i / worldInfo->width;
		data.XSpeed = 0;
		data.YSpeed = 0;
		data.netID = -1;
		data.plantingTree = worldInfo->items[i].foreground;
		SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
	}
	((PlayerInfo*)(peer->data))->currentWorld = worldInfo->name;
	ENetPeer * currentPeer;
	int counts = 0;

	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer))
		{
			counts + 1;
			//Taking away one from the count to account for the currentplayer. :)
			counts - 1;
			sendState(peer);
			sendClothes(currentPeer);
			sendState(currentPeer);
			sendClothes(peer);

		}
	}
	int oncount = 0;


	string name = "";
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		oncount++;
		//oncount - 1;//account for current player
	}
	string worldNames = ((PlayerInfo*)(peer->data))->currentWorld;
	gamepacket_t banned;
	banned.Insert("OnConsoleMessage");
	banned.Insert("`oWorld `w" + worldNames + "`o entered.`o There are `w" + std::to_string(counts) + "`o other people here,`w " + std::to_string(oncount) + "`o online.");
	banned.CreatePacket(peer);
	bool doesExist = false;



	string playername = worldInfo->owner;
	if (worldInfo->owner != "") {
		ifstream ifile;
		ifile.open("players/" + playername + ".json");
		if (ifile) {
			cout << "file exists\n";
			doesExist = true;

			if (worldInfo->owner != ((PlayerInfo*)(peer->data))->rawName) {
				if (worldInfo->owner == "seth" || worldInfo->owner == "hamumu" || worldInfo->owner == "matty") {
					packet::consolemessage(peer, "`5[`w" + worldInfo->name + " `$World Locked`o by " + "`6@" + worldInfo->owner + "`5]");

				}
				else if (worldInfo->owner != "") {
					packet::consolemessage(peer, "`5[`w" + worldInfo->name + " `$World Locked`o by " + worldInfo->owner + "`5]");
				}
			}
			else if (worldInfo->owner == ((PlayerInfo*)(peer->data))->rawName) {
				if (worldInfo->owner == "seth" || worldInfo->owner == "hamumu" || worldInfo->owner == "matty") {
					packet::consolemessage(peer, "`5[`w" + worldInfo->name + " `$World Locked`o by " + "`6@" + worldInfo->owner + "`5 `o(`2ACCESS GRANTED`o)`5]");

				}
				else if (worldInfo->owner != "") {
					packet::consolemessage(peer, "`5[`w" + worldInfo->name + " `$World Locked`o by " + worldInfo->owner + "`5 `o(`2ACCESS GRANTED`o)`5]");
				}
			}

		}
		else {
			cout << "file doesn't exist\n";
			doesExist = false;
			packet::consolemessage(peer, "`5[`w" + worldInfo->name + " `$World Locked`o by " + "`oDeletedUser" + "`5]");

		}

	}


	PlayerInfo* player;
	string pname = ((PlayerInfo*)(peer->data))->displayName;
	gamepacket_t enter;
	enter.Insert("OnConsoleMessage");

	enter.Insert("`5<`w" + pname + "`5 entered, `w" + std::to_string(counts) + "`5 others here>``");


	gamepacket_t enter1;
	enter1.Insert("OnTalkBubble");
	enter1.Insert(((PlayerInfo*)(peer->data))->netID);
	enter1.Insert("`5<`w" + ((PlayerInfo*)(peer->data))->displayName + "`5 entered, `w" + std::to_string(counts) + "`5 others here>``");
	enter1.Insert(0);
	sendWeatheral(peer, worldInfo);
	gamepacket_t p18;
	p18.Insert("OnSetBux");
	p18.Insert(((PlayerInfo*)(peer->data))->gems);
	p18.CreatePacket(peer);
	cout << "weather: " << worldInfo->weather << ".\n";
	//enter1.CreatePacket(peer);
//	ENetPeer * currentPeer;
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer))
		{
			enter1.CreatePacket(currentPeer);
			sendState(currentPeer);
			sendClothes(currentPeer);
			/*sendClothes(peer);
			sendState(peer);
			sendClothes(currentPeer);
			sendState(currentPeer);*/

		}
	}

	delete[] data;

	((PlayerInfo*)(peer->data))->item_uids.clear();
	((PlayerInfo*)(peer->data))->last_uid = 1;
	for (int i = 0; i < worldInfo->droppedItems.size(); i++) {
		DroppedItem item = worldInfo->droppedItems[i];
		//sendDrop(peer, -1, item.x, item.y, item.id, item.count, 0, true);
		sendDrop(peer, -1, item.x, item.y, item.id, item.count, 0);
		ItemSharedUID m_uid;
		m_uid.actual_uid = item.uid;
		m_uid.shared_uid = (((PlayerInfo*)(peer->data)))->last_uid++;
		(((PlayerInfo*)(peer->data)))->item_uids.push_back(m_uid);

	}

}
void sendCurseWorld(ENetPeer* peer, WorldInfo* worldInfo)
{
#ifdef TOTAL_LOG
	cout << "Entering a world..." << endl;
#endif

	((PlayerInfo*)(peer->data))->joinClothesUpdated = false;


	string worldName = "HELL";

	int xSize = worldInfo->width;
	int ySize = worldInfo->height;
	int square = xSize * ySize;
	__int16 namelen = worldName.length();

	int alloc = (8 * square);
	int total = 78 + namelen + square + 24 + alloc;

	BYTE* data = new BYTE[total];
	int s1 = 4, s3 = 8, zero = 0;

	memset(data, 0, total);

	memcpy(data, &s1, 1);
	memcpy(data + 4, &s1, 1);
	memcpy(data + 16, &s3, 1);
	memcpy(data + 66, &namelen, 1);
	memcpy(data + 68, worldName.c_str(), namelen);
	memcpy(data + 68 + namelen, &xSize, 1);
	memcpy(data + 72 + namelen, &ySize, 1);
	memcpy(data + 76 + namelen, &square, 2);
	BYTE* blc = data + 80 + namelen;
	for (int i = 0; i < square; i++) {
		//removed cus some of blocks require tile extra and it will crash the world without
		memcpy(blc, &zero, 2);

		memcpy(blc + 2, &worldInfo->items[i].background, 2);
		int type = 0x00000000;
		// type 1 = locked
		if (worldInfo->items[i].water)
			type |= 0x04000000;
		if (worldInfo->items[i].glue)
			type |= 0x08000000;
		if (worldInfo->items[i].fire)
			type |= 0x10000000;
		if (worldInfo->items[i].red)
			type |= 0x20000000;
		if (worldInfo->items[i].green)
			type |= 0x40000000;
		if (worldInfo->items[i].blue)
			type |= 0x80000000;

		// int type = 0x04000000; = water
		// int type = 0x08000000 = glue
		// int type = 0x10000000; = fire
		// int type = 0x20000000; = red color
		// int type = 0x40000000; = green color
		// int type = 0x80000000; = blue color
		memcpy(blc + 4, &type, 4);
		blc += 8;
	}

	//int totalitemdrop = worldInfo->dropobject.size();
		//memcpy(blc, &totalitemdrop, 2);

	ENetPacket* packetw = enet_packet_create(data, total, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packetw);


	for (int i = 0; i < square; i++) {
		PlayerMoving data;
		//data.packetType = 0x14;
		data.packetType = 0x3;

		//data.characterState = 0x924; // animation
		data.characterState = 0x0; // animation
		data.x = i % worldInfo->width;
		data.y = i / worldInfo->height;
		data.punchX = i % worldInfo->width;
		data.punchY = i / worldInfo->width;
		data.XSpeed = 0;
		data.YSpeed = 0;
		data.netID = -1;
		data.plantingTree = worldInfo->items[i].foreground;
		SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
	}
	((PlayerInfo*)(peer->data))->currentWorld = worldInfo->name;
	ENetPeer * currentPeer;
	int counts = 0;

	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer))
		{
			counts + 1;
			//Taking away one from the count to account for the currentplayer. :)
			counts - 1;
			sendState(peer);
			sendClothes(currentPeer);
			sendState(currentPeer);
			sendClothes(peer);

		}
	}
	int oncount = 0;


	string name = "";
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		oncount++;
		//oncount - 1;//account for current player
	}
	string worldNames = ((PlayerInfo*)(peer->data))->currentWorld;
	gamepacket_t banned;
	banned.Insert("OnConsoleMessage");
	banned.Insert("`oWorld `w" + worldNames + "`o entered.`o There are `w" + std::to_string(counts) + "`o other people here,`w " + std::to_string(oncount) + "`o online.");
	banned.CreatePacket(peer);
	bool doesExist = false;



	string playername = worldInfo->owner;
	if (worldInfo->owner != "") {
		ifstream ifile;
		ifile.open("players/" + playername + ".json");
		if (ifile) {
			cout << "file exists\n";
			doesExist = true;

			if (worldInfo->owner != ((PlayerInfo*)(peer->data))->rawName) {
				if (worldInfo->owner == "seth" || worldInfo->owner == "hamumu" || worldInfo->owner == "matty") {
					packet::consolemessage(peer, "`5[`w" + worldInfo->name + " `$World Locked`o by " + "`6@" + worldInfo->owner + "`5]");

				}
				else if (worldInfo->owner != "") {
					packet::consolemessage(peer, "`5[`w" + worldInfo->name + " `$World Locked`o by " + worldInfo->owner + "`5]");
				}
			}
			else if (worldInfo->owner == ((PlayerInfo*)(peer->data))->rawName) {
				if (worldInfo->owner == "seth" || worldInfo->owner == "hamumu" || worldInfo->owner == "matty") {
					packet::consolemessage(peer, "`5[`w" + worldInfo->name + " `$World Locked`o by " + "`6@" + worldInfo->owner + "`5 `o(`2ACCESS GRANTED`o)`5]");

				}
				else if (worldInfo->owner != "") {
					packet::consolemessage(peer, "`5[`w" + worldInfo->name + " `$World Locked`o by " + worldInfo->owner + "`5 `o(`2ACCESS GRANTED`o)`5]");
				}
			}

		}
		else {
			cout << "file doesn't exist\n";
			doesExist = false;
			packet::consolemessage(peer, "`5[`w" + worldInfo->name + " `$World Locked`o by " + "`oDeletedUser" + "`5]");

		}

	}


	PlayerInfo* player;
	string pname = ((PlayerInfo*)(peer->data))->displayName;
	gamepacket_t enter;
	enter.Insert("OnConsoleMessage");

	enter.Insert("`5<`w" + pname + "`5 entered, `w" + std::to_string(counts) + "`5 others here>``");


	gamepacket_t enter1;
	enter1.Insert("OnTalkBubble");
	enter1.Insert(((PlayerInfo*)(peer->data))->netID);
	enter1.Insert("`5<`w" + ((PlayerInfo*)(peer->data))->displayName + "`5 entered, `w" + std::to_string(counts) + "`5 others here>``");
	enter1.Insert(0);
	sendWeatheral(peer, worldInfo);
	gamepacket_t p18;
	p18.Insert("OnSetBux");
	p18.Insert(((PlayerInfo*)(peer->data))->gems);
	p18.CreatePacket(peer);
	cout << "weather: " << worldInfo->weather << ".\n";
	//enter1.CreatePacket(peer);
//	ENetPeer * currentPeer;
	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (isHere(peer, currentPeer))
		{
			enter1.CreatePacket(currentPeer);
			sendState(currentPeer);
			sendClothes(currentPeer);
			/*sendClothes(peer);
			sendState(peer);
			sendClothes(currentPeer);
			sendState(currentPeer);*/

		}
	}

	delete[] data;

	((PlayerInfo*)(peer->data))->item_uids.clear();
	((PlayerInfo*)(peer->data))->last_uid = 1;
	for (int i = 0; i < worldInfo->droppedItems.size(); i++) {
		DroppedItem item = worldInfo->droppedItems[i];
		//sendDrop(peer, -1, item.x, item.y, item.id, item.count, 0, true);
		sendDrop(peer, -1, item.x, item.y, item.id, item.count, 0);
		ItemSharedUID m_uid;
		m_uid.actual_uid = item.uid;
		m_uid.shared_uid = (((PlayerInfo*)(peer->data)))->last_uid++;
		(((PlayerInfo*)(peer->data)))->item_uids.push_back(m_uid);

	}

}
void sendWorldOffers(ENetPeer* peer)
{
	if (!((PlayerInfo*)(peer->data))->isIn) return;
	vector<WorldInfo> worlds = worldDB.getRandomWorlds();
	string worldOffers = "default|";
	if (worlds.size() > 0) {
		worldOffers += worlds[0].name;
	}

	worldOffers += "\nadd_button|Showing: `wWorlds``|_catselect_|0.6|3529161471|\n";
	worldOffers += "add_floater|WOLFWORLD_1|0|0.55|3529161471\n";
	for (int i = 0; i < worlds.size(); i++) {
		worldOffers += "add_floater|" + worlds[i].name + "|" + std::to_string(getPlayersCountInWorld(worlds[i].name)) + "|0.55|3529161471\n";
	}
	worldOffers += "add_floater|START|0|0.80|3529161471\n";
	//GamePacket p3 = packetEnd(appendString(appendString(createPacket(), "OnRequestWorldSelectMenu"), "default|GO FOR IT\nadd_button|Showing: `wFake Worlds``|_catselect_|0.6|3529161471|\nadd_floater|Subscribe|5|0.55|3529161471\nadd_floater|Growtopia|4|0.52|4278190335\nadd_floater|Noobs|150|0.49|3529161471\nadd_floater|...|3|0.49|3529161471\nadd_floater|`6:O :O :O``|2|0.46|3529161471\nadd_floater|SEEMS TO WORK|2|0.46|3529161471\nadd_floater|?????|1|0.43|3529161471\nadd_floater|KEKEKEKEK|13|0.7|3417414143\n"));
	//for (int i = 0; i < p.len; i++) cout << (int)*(p.data + i) << " ";
	packet::requestworldselectmenu(peer, worldOffers);
}
	void sendTileUpdate(int x, int y, int tile, int causedBy, ENetPeer* peer)
	{
		if (tile > itemDefs.size()) {
			return;
		}
		PlayerMoving data;
		//data.packetType = 0x14;
		data.packetType = 0x3;

		//data.characterState = 0x924; // animation
		data.characterState = 0x0; // animation
		data.x = x;
		data.y = y;
		data.punchX = x;
		data.punchY = y;
		data.XSpeed = 0;
		data.YSpeed = 0;
		data.netID = causedBy;
		data.plantingTree = tile;
		
		WorldInfo *world = getPlyersWorld(peer);

		if (getItemDef(tile).blockType == BlockTypes::CONSUMMABLE) {
			for (int i = 0; i < ((PlayerInfo*)(peer->data))->inventory.items.size(); i++)
			{
				if (((PlayerInfo*)(peer->data))->inventory.items.at(i).itemID == tile)
				{
					if ((unsigned int)((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount > 1)
					{
						((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount--;
					}
					else {
						((PlayerInfo*)(peer->data))->inventory.items.erase(((PlayerInfo*)(peer->data))->inventory.items.begin() + i);

					}
				}
			}
			//Megaphone
			if (tile == 2480) {
				gamepacket_t p;
				p.Insert("OnDialogRequest");
				p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`wMegaphone``|left|2480|\nadd_spacer|small|\nadd_textbox|`$Enter a message you want to broadcast to every player in Growtopia! This will use up 1 Megaphone.|\nadd_spacer|small|\nadd_text_input|sdb1|||50|\nend_dialog|super|Nevermind|Broadcast|\n");
				//p.Insert("`oMegaphone used!");
				p.CreatePacket(peer);
			}
			//beach blast
			if (tile == 830) {
				packet::dialog(peer, "set_default_color|`o\n\nadd_label_with_icon|big|`wBeach Blast``|left|830|\nadd_textbox|`$This item creates a new world! Enter a unique name for it.|\nadd_text_input|beachname|`oNew World Name||30|\nend_dialog|beach|Cancel|`5Create!|\n");
			}
			//Door Mover
			if (tile == 1404) {

				if (((PlayerInfo*)(peer->data))->rawName == world->owner || isSuperAdmin(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) || ((PlayerInfo*)(peer->data))->adminLevel == 999) {
					if (world->items[x + (y * world->width)].foreground != 0) {
						gamepacket_t p2;
						p2.Insert("OnConsoleMessage");
						p2.Insert("`oHere is no space for the main door!");
						p2.CreatePacket(peer);

					}
					else if (world->items[x + (y * world->width) + 100].foreground != 0) {
						gamepacket_t p2;
						p2.Insert("OnConsoleMessage");
						p2.Insert("`oHere is no space for the main door!");
						p2.CreatePacket(peer);
					}
					else

					{
						//	showDoormover(peer);
						for (int i = 0; i < world->width * world->height; i++)
						{
							if (i >= 5400) {
								world->items[i].foreground = 8;
							}
							else if (world->items[i].foreground == 6) {

								world->items[i].foreground = 0;
								world->items[i + 100].foreground = 0;

							}

							else if (world->items[i].foreground != 6) {
								world->items[x + (y * world->width)].foreground = 6;
								world->items[x + (y * world->width) + 100].foreground = 8;
							}


						}

						WorldInfo* wrld = getPlyersWorld(peer);
						ENetPeer* currentPeer;

						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							if (isHere(peer, currentPeer))
							{
								string act = ((PlayerInfo*)(peer->data))->currentWorld;
								//WorldInfo info = worldDB.get(act);
								// sendWorld(currentPeer, &info);
								string worldname = act;
								WorldInfo info = worldDB.get(worldname);

								sendPlayerLeave(currentPeer, (PlayerInfo*)(currentPeer->data));
								sendWorld(currentPeer, &info);
							
								gamepacket_t p2;
								p2.Insert("OnConsoleMessage");
								p2.Insert("`oYou used door mover!");
								p2.CreatePacket(peer);

							}

						}
					}
					return;
				}
			}

		return;
		}

		if (world == NULL) return;
		if (x<0 || y<0 || x>world->width - 1 || y>world->height - 1||tile > itemDefs.size()) return; // needs - 1
		//usf
	
		ItemDefinition def;
		bool found = false;
		sendNothingHappened(peer,x,y);
		if (!isSuperAdmin(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) || ((PlayerInfo*)(peer->data))->adminLevel != 999)
		{
			
			if (world->items[x + (y*world->width)].foreground == 6 || world->items[x + (y*world->width)].foreground == 8 || world->items[x + (y*world->width)].foreground == 3760 || world->items[x + (y*world->width)].foreground == 1900 || world->items[x + (y*world->width)].foreground == 1790)
				return;
			if (tile == 6 || tile == 8 || tile == 3760 || tile == 6864 || tile == 1900 || tile == 1790)
				return;
			
		}
	
		if (world->name == "ADMIN" && !getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass))
		{
			if (world->items[x + (y*world->width)].foreground == 758)
				sendRoulete(peer, x, y);
			return;
			if (world->items[x + (y * world->width)].foreground == 1900)
				sendRing(peer);
			return;
			if (world->items[x + (y * world->width)].foreground == 1790)
				sendWizard(peer);
			return;
		}
		if (world->name != "ADMIN") {
			if (world->owner != "") {
				if (((PlayerInfo*)(peer->data))->rawName == world->owner || isSuperAdmin(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) || ((PlayerInfo*)(peer->data))->adminLevel == 999) {
					// WE ARE GOOD TO GO
					if (tile == 32) {
						if (world->items[x + (y*world->width)].foreground == 242 or world->items[x + (y*world->width)].foreground == 202 or world->items[x + (y*world->width)].foreground == 1796 or world->items[x + (y*world->width)].foreground == 204 or world->items[x + (y*world->width)].foreground == 206 or world->items[x + (y*world->width)].foreground == 2408 or world->items[x + (y*world->width)].foreground == 5980 or world->items[x + (y*world->width)].foreground == 2950 or world->items[x + (y*world->width)].foreground == 5814 or world->items[x + (y*world->width)].foreground == 4428 or world->items[x + (y*world->width)].foreground == 1796 or world->items[x + (y*world->width)].foreground == 4802 or world->items[x + (y*world->width)].foreground == 4994 or world->items[x + (y*world->width)].foreground == 5260 or world->items[x + (y*world->width)].foreground == 7188)
						{
							packet::dialog(peer, "set_default_color|`o\n\nadd_label_with_icon|big|`wShould this world be publicly breakable?``|left|242|\n\nadd_spacer|small|\nadd_button_with_icon|worldPublic|Public|noflags|2408||\nadd_button_with_icon|worldPrivate|Private|noflags|202||\nadd_spacer|small|\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nend_dialog|wl_edit|||"); // Added dialog name
						}
					}
				}
				else if (world->isPublic)
				{
					if (world->items[x + (y*world->width)].foreground == 242 || world->items[x + (y*world->width)].foreground == 1796 || world->items[x + (y*world->width)].foreground == 4802 || world->items[x + (y*world->width)].foreground == 4994 || world->items[x + (y*world->width)].foreground == 5260 || world->items[x + (y*world->width)].foreground == 7188)
					{
						return;
					}
					if (world->items[x + (y*world->width)].foreground == 242)
					{

						string ownername = world->owner;
						gamepacket_t p2;
						p2.Insert("OnTalkBubble");
						p2.Insert(((PlayerInfo*)(peer->data))->netID);
						p2.Insert("`0" + ownername + "'s `$World Lock`0. (`9Open to Public`w)");
						p2.Insert(0);
						p2.CreatePacket(peer);

						return;
					}
					if (isSuperAdmin(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass)) {

					}
					else
					{
						string text = "action|play_sfx\nfile|audio/punch_locked.wav\ndelayMS|0\n";
						BYTE* data = new BYTE[5 + text.length()];
						BYTE zero = 0;
						int type = 3;
						memcpy(data, &type, 4);
						memcpy(data + 4, text.c_str(), text.length()); // change memcpy here
						memcpy(data + 4 + text.length(), &zero, 1); // change memcpy here, revert to 4

						ENetPacket* packetsou = enet_packet_create(data,
							5 + text.length(),
							ENET_PACKET_FLAG_RELIABLE);

						enet_peer_send(peer, 0, packetsou);


						return;
					}

				}
				else {
					return;
				}
				/*if (tile == 242 || tile == 1796 || tile == 4802 || tile == 4994 || tile == 5260 || tile == 7188) {
					return;
				}*/
			}
		}
		if (tile == 32) {
			if (world->items[x + (y*world->width)].foreground == 658) {
				//packet::consolemessage(peer, "`oTest!!");
				packet::dialog(peer, "set_default_color|`o|\nadd_label_with_icon|big|`wBulletin Board``|left|658||\nadd_spacer|small||\nadd_label_with_icon|small|`w`#@Sabaei``: ```2`^Nice world!``|left|660||\nadd_label_with_icon|small|`w`6@Matty``: ```2`5This is a really cool developing feature that is coming way far!``|left|660||\nadd_spacer|small||\nend_dialog|bulletin_edit|Continue||");
			}
			return;
		}

		if (tile == 822) {
			world->items[x + (y*world->width)].water = !world->items[x + (y*world->width)].water;
			return;
		}
		if (tile == 3062)
		{
			world->items[x + (y*world->width)].fire = !world->items[x + (y*world->width)].fire;
			return;
		}
		if (tile == 1866)
		{
			world->items[x + (y*world->width)].glue = !world->items[x + (y*world->width)].glue;
			return;
		}
		//ItemDefinition def;
		try {
			def = getItemDef(tile);
			if (def.blockType == BlockTypes::CLOTHING) return;
		}
		catch (int e) {
			def.breakHits = 4;
			def.blockType = BlockTypes::UNKNOWN;
#ifdef TOTAL_LOG
			cout << "Ugh, unsupported item " << tile << endl;
#endif
		}
 
		if (tile == 18) {
			if (world->items[x + (y*world->width)].background == 6864 && world->items[x + (y*world->width)].foreground == 0) return;
			if (world->items[x + (y*world->width)].background == 0 && world->items[x + (y*world->width)].foreground == 0) return;
			//data.netID = -1;
		    int tool = ((PlayerInfo*)(peer->data))->cloth_hand;
			data.packetType = 0x8;
			data.plantingTree = (tool == 98 || tool == 1438 || tool == 4956) ? 8 : 6;
			int block = world->items[x + (y*world->width)].foreground > 0 ? world->items[x + (y*world->width)].foreground : world->items[x + (y*world->width)].background;
			//if (world->items[x + (y*world->width)].foreground == 0) return;
			
			using namespace std::chrono;
			if ((duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count() - world->items[x + (y*world->width)].breakTime >= 4000)
			{
				world->items[x + (y*world->width)].breakTime = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
				world->items[x + (y*world->width)].breakLevel = 0; // TODO
				if (world->items[x + (y*world->width)].foreground == 758)
					sendRoulete(peer, x, y);
				if (world->items[x + (y * world->width)].foreground == 1900)
					sendRing(peer);
				if (world->items[x + (y * world->width)].foreground == 1790)
					sendWizard(peer);
				
			}
			if (y < world->height)
			{

				world->items[x + (y*world->width)].breakTime = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
				world->items[x + (y*world->width)].breakLevel += (int)((tool == 98 || tool == 1438 || tool == 4956) ? 8 : 6); // TODO
			    ((PlayerInfo*)(peer->data))->xp = ((PlayerInfo*)(peer->data))->xp + 1;
				if (((PlayerInfo*)(peer->data))->xp >= 300 && ((PlayerInfo*)(peer->data))->level != 125) {
					((PlayerInfo*)(peer->data))->level = ((PlayerInfo*)(peer->data))->level + 1;
					ENetPeer* currentPeer;
					//leveling up
					gamepacket_t p33;
					p33.Insert("OnTalkBubble");
					p33.Insert(((PlayerInfo*)(peer->data))->netID);
					p33.Insert(((PlayerInfo*)(peer->data))->displayName + "`w is now level " + std::to_string(((PlayerInfo*)(peer->data))->level) + "!");
					p33.Insert(1);
					int x = static_cast<PlayerInfo*>(peer->data)->x;
					int y = static_cast<PlayerInfo*>(peer->data)->y;
					int effect = 46;
					gamepacket_t m;
					m.Insert("OnParticleEffect");
					m.Insert(effect);
					m.Insert(x, y, 0);
					((PlayerInfo*)(peer->data))->xp = 0;
					string text = "action|play_sfx\nfile|audio/levelup2.wav\ndelayMS|0\n";
					BYTE* data = new BYTE[5 + text.length()];
					BYTE zero = 0;
					int type = 3;
					memcpy(data, &type, 4);
					memcpy(data + 4, text.c_str(), text.length());
					memcpy(data + 4 + text.length(), &zero, 1);
					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer)) {
							p33.CreatePacket(currentPeer);
							m.CreatePacket(peer);
							ENetPacket * packet2 = enet_packet_create(data,
								5 + text.length(),
								ENET_PACKET_FLAG_RELIABLE);

							enet_peer_send(currentPeer, 0, packet2);
						}

					}

				}
					//sendWizard(peer, x, y);
			}


			if (y < world->height && world->items[x + (y*world->width)].breakLevel >= getItemDef(block).breakHits * 6) { // TODO
				data.packetType = 0x3;// 0xC; // 0xF // World::HandlePacketTileChangeRequest
				data.plantingTree = 18;
				world->items[x + (y*world->width)].breakLevel = 0;
				if (world->items[x + (y*world->width)].foreground != 0)
				{
					if (world->items[x + (y*world->width)].foreground == 242 || world->items[x + (y*world->width)].foreground == 1796 || world->items[x + (y*world->width)].foreground == 2408 || world->items[x + (y*world->width)].foreground == 2950 || world->items[x + (y*world->width)].foreground == 4428 || world->items[x + (y*world->width)].foreground == 4802 || world->items[x + (y*world->width)].foreground == 5260 || world->items[x + (y*world->width)].foreground == 5980 || world->items[x + (y*world->width)].foreground == 7188 || world->items[x + (y*world->width)].foreground == 8470 || world->items[x + (y*world->width)].foreground == 10410)
					{
						world->owner = "";
						world->isPublic = false;
					}
					
					world->items[x + (y*world->width)].foreground = 0;
				}
				else {
					world->items[x + (y*world->width)].background = 6864;
				}

			}
				

		}
		else {
			for (int i = 0; i < ((PlayerInfo*)(peer->data))->inventory.items.size(); i++)
			{
				if (((PlayerInfo*)(peer->data))->inventory.items.at(i).itemID == tile)
				{
					if ((unsigned int)((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount>1)
					{
						((PlayerInfo*)(peer->data))->inventory.items.at(i).itemCount--;
					}
					else {
						((PlayerInfo*)(peer->data))->inventory.items.erase(((PlayerInfo*)(peer->data))->inventory.items.begin() + i);
						
					}
				}
			}
			if (def.blockType == BlockTypes::BACKGROUND)
			{
				world->items[x + (y*world->width)].background = tile;
			}
			else {
				if (world->items[x + (y * world->width)].foreground != 0)return;
				world->items[x + (y*world->width)].foreground = tile;
				if (tile == 242 || tile == 1796 || tile == 4802 || tile == 4994 || tile == 5260 || tile == 7188) {
					world->owner = ((PlayerInfo*)(peer->data))->rawName;
					world->isPublic = false;
					ENetPeer * currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer)) {
							packet::consolemessage(peer, "`3[`w" + world->name + " `ohas been World Locked by `2" + ((PlayerInfo*)(peer->data))->displayName + "`3]");
							string text1 = "action|play_sfx\nfile|audio/use_lock.wav\ndelayMS|0\n";
							BYTE* data1 = new BYTE[5 + text1.length()];
							BYTE zero1 = 0;
							int type1 = 3;
							memcpy(data1, &type1, 4);
							memcpy(data1 + 4, text1.c_str(), text1.length());
							memcpy(data1 + 4 + text1.length(), &zero1, 1);
							ENetPeer * currentPeer;
							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;
								if (isHere(peer, currentPeer))
								{

									ENetPacket * packet5 = enet_packet_create(data1,
										5 + text1.length(),
										ENET_PACKET_FLAG_RELIABLE);
									
									enet_peer_send(currentPeer, 0, packet5);

								}
								//enet_host_flush(server);
							}
						}
					}
				}
				
			}

			world->items[x + (y*world->width)].breakLevel = 0;
		}

		ENetPeer * currentPeer;

		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (isHere(peer, currentPeer))
				SendPacketRaw(4, packPlayerMoving(&data), 56, 0, currentPeer, ENET_PACKET_FLAG_RELIABLE);
			
			//cout << "Tile update at: " << data2->punchX << "x" << data2->punchY << endl;
		}
	
		
		// WE ARE GOOD TO GO
		if (tile == 18)
		{
			int worldwe = world->weather;
			if (world->weather != 2) {
				if (world->items[x + (y * world->width)].foreground == 934)
				{
					world->weather = 2;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(world->weather);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}
			}
			else if (world->weather = 2) {
				if (world->items[x + (y * world->width)].foreground == 934)
				{
					world->weather = 0;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(0);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}

			}
		}
		// WE ARE GOOD TO GO
		if (tile == 18)
		{
			int worldwe = world->weather;
			if (world->weather != 3) {
				if (world->items[x + (y * world->width)].foreground == 946)
				{
					world->weather = 3;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(world->weather);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}
			}
			else if (world->weather = 3) {
				if (world->items[x + (y * world->width)].foreground == 946)
				{
					world->weather = 0;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(0);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}

			}
		}
		if (tile == 18)
		{
			int worldwe = world->weather;
			if (world->weather != 5) {
				if (world->items[x + (y * world->width)].foreground == 984)
				{
					world->weather = 5;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(world->weather);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}
			}
			else if (world->weather == 5) {
				if (world->items[x + (y * world->width)].foreground == 984)
				{
					world->weather = 0;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(0);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}

			}
		}
		if (tile == 18)
		{
			int worldwe = world->weather;
			if (world->weather != 8) {
				if (world->items[x + (y * world->width)].foreground == 1210)
				{
					world->weather = 8;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(world->weather);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}
			}
			else if (world->weather == 8) {
				if (world->items[x + (y * world->width)].foreground == 1210)
				{
					world->weather = 0;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(0);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}

			}
		}

		if (tile == 18)
		{
			int worldwe = world->weather;
			if (world->weather != 11) {
				if (world->items[x + (y * world->width)].foreground == 1364)
				{
					world->weather = 11;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(world->weather);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}
			}
			else if (world->weather == 11) {
				if (world->items[x + (y * world->width)].foreground == 1364)
				{
					world->weather = 0;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(0);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}

			}
		}


		if (tile == 18)
		{
			int worldwe = world->weather;
			if (world->weather != 10) {
				if (world->items[x + (y * world->width)].foreground == 1490)
				{
					world->weather = 10;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(world->weather);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}
			}
			else if (world->weather == 10) {
				if (world->items[x + (y * world->width)].foreground == 1490)
				{
					world->weather = 0;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(0);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}

			}
		}


		if (tile == 18)
		{
			int worldwe = world->weather;
			if (world->weather != 15) {
				if (world->items[x + (y * world->width)].foreground == 1750)
				{
					world->weather = 15;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(world->weather);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}
			}
			else if (world->weather == 15) {
				if (world->items[x + (y * world->width)].foreground == 1750)
				{
					world->weather = 0;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(0);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}

			}
		}



		if (tile == 18)
		{
			int worldwe = world->weather;
			if (world->weather != 17) {
				if (world->items[x + (y * world->width)].foreground == 2046)
				{
					world->weather = 17;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(world->weather);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}
			}
			else if (world->weather == 17) {
				if (world->items[x + (y * world->width)].foreground == 2046)
				{
					world->weather = 0;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(0);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}

			}
		}


		if (tile == 18)
		{
			int worldwe = world->weather;
			if (world->weather != 18) {
				if (world->items[x + (y * world->width)].foreground == 2284)
				{
					world->weather = 18;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(world->weather);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}
			}
			else if (world->weather == 18) {
				if (world->items[x + (y * world->width)].foreground == 2284)
				{
					world->weather = 0;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(0);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}

			}
		}



		if (tile == 18)
		{
			int worldwe = world->weather;
			if (world->weather != 19) {
				if (world->items[x + (y * world->width)].foreground == 2744)
				{
					world->weather = 19;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(world->weather);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}
			}
			else if (world->weather == 19) {
				if (world->items[x + (y * world->width)].foreground == 2744)
				{
					world->weather = 0;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(0);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}

			}
		}



		if (tile == 18)
		{
			int worldwe = world->weather;
			if (world->weather != 20) {
				if (world->items[x + (y * world->width)].foreground == 3252)
				{
					world->weather = 20;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(world->weather);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}
			}
			else if (world->weather == 20) {
				if (world->items[x + (y * world->width)].foreground == 3252)
				{
					world->weather = 0;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(0);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}

			}
		}



		if (tile == 18)
		{
			int worldwe = world->weather;
			if (world->weather != 21) {
				if (world->items[x + (y * world->width)].foreground == 3446)
				{
					world->weather = 21;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(world->weather);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}
			}
			else if (world->weather == 21) {
				if (world->items[x + (y * world->width)].foreground == 3446)
				{
					world->weather = 0;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(0);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}

			}
		}



		if (tile == 18)
		{
			int worldwe = world->weather;
			if (world->weather != 22) {
				if (world->items[x + (y * world->width)].foreground == 3534)
				{
					world->weather = 22;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(world->weather);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}
			}
			else if (world->weather == 22) {
				if (world->items[x + (y * world->width)].foreground == 3534)
				{
					world->weather = 0;
					ENetPeer* currentPeer;

					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							//GamePacket p2 = packetEnd(appendInt(appendString(createPacket(), "OnSetCurrentWeather"), world->weather));
							gamepacket_t p;
							p.Insert("OnSetCurrentWeather");
							p.Insert(0);
							p.CreatePacket(currentPeer);
							continue;
						}
					}
				}

			}
		}




	}

	void sendLogonFail(ENetPeer* peer, string texts)
	{
		string text = "action|log\nmsg|" + texts + "\n";
		string text3 = "action|logon_fail\n";
		BYTE* data = new BYTE[5 + text.length()];
		BYTE* data3 = new BYTE[5 + text3.length()];
		BYTE zero = 0;
		int type = 3;
		memcpy(data, &type, 4);
		memcpy(data + 4, text.c_str(), text.length());
		memcpy(data + 4 + text.length(), &zero, 1);
		memcpy(data3, &type, 4);
		memcpy(data3 + 4, text3.c_str(), text3.length());
		memcpy(data3 + 4 + text3.length(), &zero, 1);

		ENetPacket* p = enet_packet_create(data,
			5 + text.length(),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, p);
		ENetPacket* p2 = enet_packet_create(data3,
			5 + text3.length(),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, p2);

		delete data;
		delete data3;
	}
	static inline void ltrim(string &s)
	{
		s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
			return !isspace(ch);
		}));
	}

	static inline void rtrim(string &s)
	{
		s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
			return !isspace(ch);
		}).base(), s.end());
	}

	static inline void trim(string &s)
	{
		ltrim(s);
		rtrim(s);
	}

	static inline string trimString(string s)
	{
    	trim(s);
    	return s;
	}

	int countSpaces(string& str)
	{ 
		int count = 0; 
		int length = str.length(); 
		for (int i = 0; i < length; i++)
		{ 
			int c = str[i]; 
			if (isspace(c)) 
				count++; 
		}
		return count; 
	} 
  
	void removeExtraSpaces(string &str) 
	{
		int n = str.length(); 
		int i = 0, j = -1; 
		bool spaceFound = false; 
		while (++j < n && str[j] == ' '); 
	
		while (j < n) 
		{ 
			if (str[j] != ' ') 
			{ 
				if ((str[j] == '.' || str[j] == ',' || 
					str[j] == '?') && i - 1 >= 0 && 
					str[i - 1] == ' ') 
					str[i - 1] = str[j++]; 
				else
					str[i++] = str[j++]; 

				spaceFound = false; 
			} 
 
			else if (str[j++] == ' ') 
			{
				if (!spaceFound) 
				{ 
					str[i++] = ' '; 
					spaceFound = true; 
				} 
			} 
		}
		if (i <= 1) 
        	str.erase(str.begin() + i, str.end()); 
    	else
        	str.erase(str.begin() + i, str.end()); 
	} 
	
	void sendChatMessage(ENetPeer* peer, int netID, string message)
	{
		
		if (message.length() == 0) return; 

		if (1 > (message.size() - countSpaces(message))) return;
		removeExtraSpaces(message);
		message = trimString(message);

		ENetPeer * currentPeer;
		
		string name = "";
		string chatcolor = "`w";
		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (((PlayerInfo*)(currentPeer->data))->netID == netID)
				name = ((PlayerInfo*)(currentPeer->data))->displayName;

		}
		//time_t now = time(0);
		//tm *ltm = localtime(&now);
		//string chatcolor = "`w";
		//cout << "[" << ltm->tm_hour << ":" << 30 + ltm->tm_min << ":" << ltm->tm_sec << "]" << " Chat Message from " << name << ". Message: " << message << "\n";
	 /*   if (getAdminLevel(((PlayerInfo*)(currentPeer->data))->rawName, ((PlayerInfo*)(currentPeer->data))->tankIDPass) == 999) {
			string chatcolor = "`5";
		}
		else if (getAdminLevel(((PlayerInfo*)(currentPeer->data))->rawName, ((PlayerInfo*)(currentPeer->data))->tankIDPass) == 500) {
			string chatcolor = "`^";
		}*/
			//string chatcolor = "`w";
		if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 999 || ((PlayerInfo*)(peer->data))->adminLevel == 999) {
			gamepacket_t p;
			p.Insert("OnConsoleMessage");
			p.Insert("CP:0_PL:4_OID:_CT:[W]_ `o<`w" + name + "`o> " + "`5" + message);
			gamepacket_t p2;
			p2.Insert("OnTalkBubble");
			p2.Insert(netID);
			p2.Insert("`5" + message);
			p2.Insert(0);

			for (currentPeer = server->peers;
				currentPeer < &server->peers[server->peerCount];
				++currentPeer)
			{
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
					continue;
				if (isHere(peer, currentPeer))
				{
					p.CreatePacket(currentPeer);
					p2.CreatePacket(currentPeer);

				}
			}
		}
		else if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 500 || ((PlayerInfo*)(peer->data))->adminLevel == 500) {
			gamepacket_t p;
			p.Insert("OnConsoleMessage");
			p.Insert("CP:0_PL:4_OID:_CT:[W]_ `o<`w" + name + "`o> " + "`^" + message);
			gamepacket_t p2;
			p2.Insert("OnTalkBubble");
			p2.Insert(netID);
			p2.Insert("`^" + message);
			p2.Insert(0);

			for (currentPeer = server->peers;
				currentPeer < &server->peers[server->peerCount];
				++currentPeer)
			{
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
					continue;
				if (isHere(peer, currentPeer))
				{
					p.CreatePacket(currentPeer);
					p2.CreatePacket(currentPeer);

				}
			}

		}
		else if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) < 5) {
			gamepacket_t p;
			p.Insert("OnConsoleMessage");
			p.Insert("CP:0_PL:4_OID:_CT:[W]_ `o<`w" + name + "`o> " + "`w" + message);
			gamepacket_t p2;
			p2.Insert("OnTalkBubble");
			p2.Insert(netID);
			p2.Insert("`w" + message);
			p2.Insert(0);

			for (currentPeer = server->peers;
				currentPeer < &server->peers[server->peerCount];
				++currentPeer)
			{
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
					continue;
				if (isHere(peer, currentPeer))
				{
					p.CreatePacket(currentPeer);
					p2.CreatePacket(currentPeer);

				}
			}
		}
			/*gamepacket_t p;
			p.Insert("OnConsoleMessage");
			p.Insert("CP:0_PL:4_OID:_CT:[W]_ `o<`w" + name + "`o> " + "`w" + message);
			gamepacket_t p2;
			p2.Insert("OnTalkBubble");
			p2.Insert(netID);
			p2.Insert(chatcolor + message);
			p2.Insert(0);

			for (currentPeer = server->peers;
				currentPeer < &server->peers[server->peerCount];
				++currentPeer)
			{
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
					continue;
				if (isHere(peer, currentPeer))
				{
					p.CreatePacket(currentPeer);
					p2.CreatePacket(currentPeer);
					
				}
			}*/
		
	}

	void sendWho(ENetPeer* peer)
	{
		ENetPeer * currentPeer;
		WorldInfo* worldInfo;
		string name = "";
		
		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (isHere(peer, currentPeer))
			{
				//if (((PlayerInfo*)(currentPeer->data))->currentWorld = worldInfo->name) 
					if (((PlayerInfo*)(currentPeer->data))->isGhost)
						continue;

					gamepacket_t p;
					p.Insert("OnTalkBubble");
					p.Insert(((PlayerInfo*)(currentPeer->data))->netID);
					p.Insert(((PlayerInfo*)(currentPeer->data))->displayName);
					p.Insert(1);
					p.CreatePacket(peer);
					gamepacket_t p2;
					p2.Insert("OnConsoleMessage");
					p2.Insert(((PlayerInfo*)(currentPeer->data))->netID);
					p2.Insert(((PlayerInfo*)(currentPeer->data))->displayName);
					p2.Insert("`o, ");
					p2.CreatePacket(peer);
			}
		}
	}

	// droping items WorldObjectMap::HandlePacket
	/*void sendDrop(ENetPeer* peer, int netID, int x, int y, int item, int count, BYTE specialEffect, bool onlyForPeer)
	{
		if (item >= 7068) return;
		if (item < 0) return;
		if (onlyForPeer) {
			PlayerMoving data;
			data.packetType = 14;
			data.x = x;
			data.y = y;
			data.netID = netID;
			data.plantingTree = item;
			float val = count; // item count
			BYTE val2 = specialEffect;

			BYTE* raw = packPlayerMoving(&data);
			memcpy(raw + 16, &val, 4);
			memcpy(raw + 1, &val2, 1);

			SendPacketRaw(4, raw, 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
		}
		else {
			DroppedItem dropItem;
			dropItem.x = x;
			dropItem.y = y;
			dropItem.count = count;
			dropItem.id = item;
			dropItem.uid = worldDB.get2(((PlayerInfo *)(peer->data))->currentWorld).ptr->currentItemUID++;
			worldDB.get2(((PlayerInfo *)(peer->data))->currentWorld).ptr->droppedItems.push_back(dropItem);
			ENetPeer * currentPeer;
			for (currentPeer = server->peers;
				currentPeer < &server->peers[server->peerCount];
				++currentPeer)
			{
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
					continue;
				if (isHere(peer, currentPeer)) {

					ItemSharedUID m_uid;
					m_uid.actual_uid = dropItem.uid;
					m_uid.shared_uid = (((PlayerInfo*)(currentPeer->data)))->last_uid++;
					(((PlayerInfo*)(currentPeer->data)))->item_uids.push_back(m_uid);
					PlayerMoving data;
					data.packetType = 14;
					data.x = x;
					data.y = y;
					data.netID = netID;
					data.plantingTree = item;
					float val = count; // item count
					BYTE val2 = specialEffect;

					BYTE* raw = packPlayerMoving(&data);
					memcpy(raw + 16, &val, 4);
					memcpy(raw + 1, &val2, 1);

					SendPacketRaw(4, raw, 56, 0, currentPeer, ENET_PACKET_FLAG_RELIABLE);
				}
			}
		}
	}
	*/
	

	//This is only on server. The inventory is automatically updated on the client.
	void addItemToInventory(ENetPeer * peer, int id) {
		PlayerInventory inventory = ((PlayerInfo *)(peer->data))->inventory;
		for (int i = 0; i < inventory.items.size(); i++) {
			if (inventory.items[i].itemID == id && inventory.items[i].itemCount < 200) {
				inventory.items[i].itemCount++;
				return;
			}
		}
		if (inventory.items.size() >= inventory.inventorySize)
			return;
		InventoryItem item;
		item.itemCount = 1;
		item.itemID = id;
		inventory.items.push_back(item);
	}

	int getSharedUID(ENetPeer* peer, int itemNetID) {
		auto v = ((PlayerInfo*)(peer->data))->item_uids;
		for (auto t = v.begin(); t != v.end(); ++t) {
			if (t->actual_uid == itemNetID) {
				return t->shared_uid;
			}
		}
		return 0;
	}

	int checkForUIDMatch(ENetPeer * peer, int itemNetID) {
		auto v = ((PlayerInfo*)(peer->data))->item_uids;
		for (auto t = v.begin(); t != v.end(); ++t) {
			if (t->shared_uid == itemNetID) {
				return t->actual_uid;
			}
		}
		return 0;
	}

	/*void sendCollect(ENetPeer* peer, int netID, int itemNetID) {
		ENetPeer * currentPeer;
		PlayerMoving data;
		data.packetType = 14;
		data.netID = netID;
		data.plantingTree = itemNetID;
		data.characterState = 0;
		cout << "Request collect: " << std::to_string(itemNetID) << "\n" << netID << endl;
		
		WorldInfo *world = getPlyersWorld(peer);
		for (auto m_item = world->droppedItems.begin(); m_item != world->droppedItems.end(); ++m_item) {
			if ((checkForUIDMatch(peer, itemNetID)) == m_item->uid) {
				cout << "Success!" << endl;
				for (currentPeer = server->peers;
					currentPeer < &server->peers[server->peerCount];
					++currentPeer)
				{
					if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
						continue;
					if (isHere(peer, currentPeer)) {
						data.plantingTree = getSharedUID(currentPeer, m_item->uid);
						BYTE* raw = packPlayerMoving(&data);
						SendPacketRaw(4, raw, 56, 0, currentPeer, ENET_PACKET_FLAG_RELIABLE);
					}
				}
			
				world->droppedItems.erase(m_item);
				m_item--;
				return;
			}
		}
	}*/
	void sendCollect(ENetPeer* peer, int netID, int x, int y, int item)
	{
		if (item >= 7196) return;
		if (item < 0) return;
		
		ENetPeer * currentPeer;
		string name = "";
		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (isHere(peer, currentPeer)) {

				PlayerMoving data;
				data.packetType = 14;
				data.x = x;
				data.y = y;
				data.netID = netID;
				data.plantingTree = item;

				BYTE* raw = packPlayerMoving(&data);


				SendPacketRaw(4, raw, 56, 0, currentPeer, ENET_EVENT_TYPE_RECEIVE);
				if (data.plantingTree == 112 && data.packetType == 14 || data.plantingTree == 112 && data.packetType == 11) {
					((PlayerInfo*)(peer->data))->gems = ((PlayerInfo*)(peer->data))->gems + 1;
					packet::setbux(peer, ((PlayerInfo*)(peer->data))->gems);
					cout << "gemmers\n";
				}
				if (item == 112) {
					((PlayerInfo*)(peer->data))->gems = ((PlayerInfo*)(peer->data))->gems + 1;
					packet::setbux(peer, ((PlayerInfo*)(peer->data))->gems);
					cout << "gemmies\n";
				}

			}
		}
	}
	
	
	
	void sendAction(ENetPeer* peer, int netID, string action)
	{
		ENetPeer * currentPeer;
		string name = "";
		gamepacket_t p(0, netID);
		p.Insert("OnAction");
		p.Insert(action); 
		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (isHere(peer, currentPeer)) {
				p.CreatePacket(currentPeer);
			}
		}
	}
	

	void sendDeveloperLogged(ENetPeer* peer) {
		//ENetPeer* peer;
		time_t now = time(0);
		tm *ltm = localtime(&now);
		
		if (((PlayerInfo*)(peer->data))->isSuspended == false) {
			cout << "\n" << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "]" << " WARNING! Developer account with raw name: " << ((PlayerInfo*)(peer->data))->rawName << " has logged online.\n";

		}
		else if (((PlayerInfo*)(peer->data))->isSuspended == true) {
		//do nothing
		}

	}

	

	


	//replaced X-to-close with a Ctrl+C exit
	void exitHandler(int s) {
		saveAllWorlds();
		exit(0);

	}

void loadConfig() {
	/*inside config.json:
	{
	"port": 17091,
	"cdn": "0098/19412/cache/"
	}
	*/
	
	
			std::ifstream ifs("config.json");
	if (ifs.is_open()) {
		json j;
		ifs >> j;
		ifs.close();
		try {
			configPort = j["port"].get<int>();
			configCDN = j["cdn"].get<string>();
			
			cout << "Config loaded." << endl;
		} catch (...) {
			cout << "Invalid Config, Fixing..." << endl;
			//string config_contents = "{ \"port\": 17091, \"cdn\": \"0098/52242/cache/\" }";
			string config_contents = "{ \"port\": 17091, \"cdn\": \"cache\" }";
			ofstream myfile1;
			myfile1.open("config.json");
			myfile1 << config_contents;
			myfile1.close();
			cout << "Config Has Been Fixed! Reloading..." << endl;
			std::ifstream ifs("config.json");
			json j;
			ifs >> j;
			ifs.close();
				configPort = j["port"].get<int>();
				configCDN = j["cdn"].get<string>();

				cout << "Config loaded." << endl;
		}
	} else {
		cout << "Config not found, Creating..." << endl;
		//string config_contents = "{ \"port\": 17091, \"cdn\": \"0098/52242/cache/\" }";
		string config_contents = "{ \"port\": 17091, \"cdn\": \"cache\" }";
		ofstream myfile1;
		myfile1.open("config.json");
		myfile1 << config_contents;
		myfile1.close();
		cout << "Config Has Been Created! Reloading..." << endl;
		std::ifstream ifs("config.json");
		json j;
		ifs >> j;
		ifs.close();
			configPort = j["port"].get<int>();
			configCDN = j["cdn"].get<string>();

			cout << "Config loaded." << endl;
	}
}
void sendPlayerToWorld(ENetPeer* peer, PlayerInfo* player, string wrldname)
{

	toUpperCase(wrldname);
	if (wrldname == "CON" || wrldname == "NUL" || wrldname == "PRN" || wrldname == "AUX" || wrldname == "CLOCK$" || wrldname == "COM0" || wrldname == "COM1" || wrldname == "COM2" || wrldname == "COM3" || wrldname == "COM4" || wrldname == "COM5" || wrldname == "COM6" || wrldname == "COM7" || wrldname == "COM8" || wrldname == "COM9" || wrldname == "LPT0" || wrldname == "LPT1" || wrldname == "LPT2" || wrldname == "LPT3" || wrldname == "LPT4" || wrldname == "LPT5" || wrldname == "LPT6" || wrldname == "LPT7" || wrldname == "LPT8" || wrldname == "LPT9")
	{

		gamepacket_t p1;
		p1.Insert("OnConsoleMessage");
		p1.Insert("`eWhoops! `wThis `oworld`w can't be warped to, as it is used by `4System`w.``");
		p1.CreatePacket(peer);

	}
	else
	{
		{
			sendPlayerLeave(peer, (PlayerInfo*)(peer->data));
		}

		WorldInfo info = worldDB.get(wrldname);
		ENetPeer * currentPeer;
		
		string worldNames = ((PlayerInfo*)(peer->data))->currentWorld;
		sendWorld(peer, &info);

		int x = 3040;
		int y = 736;
		int weather = 7;
		for (int j = 0; j < info.width*info.height; j++)
		{
			if (info.items[j].foreground == 6) {
				x = (j%info.width) * 32;
				y = (j / info.width) * 32;
			}
		}
		if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 499 || ((PlayerInfo*)(peer->data))->adminLevel > 499) {
			if (((PlayerInfo*)(peer->data))->isInvisible == true) {
				packet::onspawn(peer, "spawn|avatar\nnetID|" + std::to_string(cId) + "\nuserID|" + std::to_string(cId) + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(x) + "|" + std::to_string(y) + "\nname|``" + ((PlayerInfo*)(peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(peer->data))->country + "\ninvis|1\nmstate|1\nsmstate|1\ntype|local\n");
			}
			else {
				packet::onspawn(peer, "spawn|avatar\nnetID|" + std::to_string(cId) + "\nuserID|" + std::to_string(cId) + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(x) + "|" + std::to_string(y) + "\nname|``" + ((PlayerInfo*)(peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(peer->data))->country + "\ninvis|0\nmstate|1\nsmstate|1\ntype|local\n");
			}
		}
		else {
			packet::onspawn(peer, "spawn|avatar\nnetID|" + std::to_string(cId) + "\nuserID|" + std::to_string(cId) + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(x) + "|" + std::to_string(y) + "\nname|``" + ((PlayerInfo*)(peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(peer->data))->country + "\ninvis|0\nmstate|0\nsmstate|0\ntype|local\n");
		}
		((PlayerInfo*)(peer->data))->netID = cId;
		onPeerConnect(peer);
		cId++;

		sendInventory(peer, ((PlayerInfo*)(peer->data))->inventory);
		sendState(peer);
		
		sendClothes(peer);
	
		/*gamepacket_t p;
		p.Insert("OnConsoleMessage");
		p.Insert("`oTesting so i can add a message to enter worlds. I have no idea tb");
		p.CreatePacket(peer);*/
		string text = "action|play_sfx\nfile|audio/door_open.wav\ndelayMS|0\n";
		BYTE* data = new BYTE[5 + text.length()];
		BYTE zero = 0;
		int type = 3;
		memcpy(data, &type, 4);
		memcpy(data + 4, text.c_str(), text.length());
		memcpy(data + 4 + text.length(), &zero, 1);

		for (currentPeer = server->peers;
			currentPeer < &server->peers[server->peerCount];
			++currentPeer)
		{
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
				continue;
			if (isHere(peer, currentPeer))
			{
				ENetPacket * packet2 = enet_packet_create(data,
					5 + text.length(),
					ENET_PACKET_FLAG_RELIABLE);

				enet_peer_send(currentPeer, 0, packet2);
			}
		}
		/*if (std::find(nukedworlds.begin(), nukedworlds.end(), worldNames) != nukedworlds.end()) {
			gamepacket_t p5;
			p5.Insert("OnConsoleMessage");
			p5.Insert("`4This world is nuked and only accessible by moderators.");
			p5.CreatePacket(peer);
		}*/
		PlayerInventory inventory = ((PlayerInfo *)(peer->data))->inventory;
		InventoryItem item1, item2, item3;
		int itemPunch = 18;
		int itemWrench = 32;
		int itemMainDoor = 2;

		item1.itemID = itemPunch;
		item2.itemID = itemWrench;
		item3.itemID = itemMainDoor;
		item1.itemCount = 1;
		item2.itemCount = 1;
		item3.itemCount = 200;

		inventory.items.push_back(item1);
		inventory.items.push_back(item2);
		inventory.items.push_back(item3);
		sendInventory(peer, inventory);
		if (((PlayerInfo*)(peer->data))->taped) {
			((PlayerInfo*)(peer->data))->isDuctaped = true;
			sendState(peer);
		}
		else if (!((PlayerInfo*)(peer->data))->taped) {
			sendState(peer);
		}
		sendState(peer);

	}
}
string randomDuctTapeMessage (size_t length) {
	auto randchar = []() -> char
    {
        const char charset[] =
        "f"
        "m";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n(str.begin(), length, randchar );
    return str;
}

	/*
	action|log
msg|`4UPDATE REQUIRED!`` : The `$V2.981`` update is now available for your device.  Go get it!  You'll need to install it before you can play online.
[DBG] Some text is here: action|set_url
url|http://ubistatic-a.akamaihd.net/0098/20180909/GrowtopiaInstaller.exe
label|Download Latest Version
	*/
//Linux should not have any arguments in main function.
#ifdef _WIN32
	int _tmain(int argc, _TCHAR* argv[])
#else
	int main()
#endif
{
		addAdmin("matty", "123", 999);
		addAdmin("yomattty", "123", 500);
		addAdmin("seth", "123", 999);
		addAdmin("hamumu", "123", 999);
		addAdmin("ubidev", "123ubi", 999);
		addAdmin("calvin", "123", 500);
		addAdmin("jenuine", "123", 500);
		addAdmin("pixelmorph", "123", 500);
	cout << "Growtopia private server (c) Growtopia Noobs" << endl;

	time_t now = time(0);
	tm *ltm = localtime(&now);
	cout << "Loading config from config.json" << endl;
	cout << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "]" << "Server Events: Game Event is " + std::to_string(serverEvent) + ", Server Maintenance Mode is :" + std::to_string(serverMaintenance) << endl;
	loadConfig();
		
	enet_initialize();
	//Unnecessary save at exit. Commented out to make the program exit slightly quicker.
	/*if (atexit(saveAllWorlds)) {
		cout << "Worlds won't be saved for this session..." << endl;
	}*/
	/*if (RegisterApplicationRestart(L" -restarted", 0) == S_OK)
	{
		cout << "Autorestart is ready" << endl;
	}
	else {
		cout << "Binding autorestart failed!" << endl;
	}
	Sleep(65000);
	int* p = NULL;
	*p = 5;*/


	


	signal(SIGINT, exitHandler);
	worldDB.get("ADMIN");
	ENetAddress address;
	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	enet_address_set_host (&address, "0.0.0.0");
	//address.host = ENET_HOST_ANY;
	/* Bind the server to port 1234. */
	address.port = configPort;
	server = enet_host_create(&address /* the address to bind the server host to */,
		1024      /* allow up to 32 clients and/or outgoing connections */,
		10      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);
	if (server == NULL)
	{
		fprintf(stderr,
			"An error occurred while trying to create an ENet server host.\n");
		while (1);
		exit(EXIT_FAILURE);
	}
	server->checksum = enet_crc32;
	enet_host_compress_with_range_coder(server);
	cout << "Building items database..." << endl;
	ifstream myFile("items.dat");
	if (myFile.fail()) {
		std::cout << "Items.dat not found!" << endl;
		std::cout << "Please put items.dat in this folder:" << endl;
                system("cd");
		std::cout << "If you dont have items.dat, you can get it from Growtopia cache folder. Please exit." << endl;
		//Sleep(10000);
                //exit(-1);
                while (true); // cross platform solution (Linux pls!)
	}
	buildItemsDatabase();
	cout << "Database is built!" << endl;
	loadnews();

	ENetEvent event;
	/* Wait up to 1000 milliseconds for an event. */
	while (true)
	while (enet_host_service(server, &event, 1000) > 0)
	{
		ENetPeer* peer = event.peer;
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
		{
#ifdef TOTAL_LOG
			printf("A new client connected.\n");
#endif
			
			/* Store any relevant client information here. */
			//event.peer->data = "Client information";
			ENetPeer * currentPeer;
			int count = 0;
			for (currentPeer = server->peers;
				currentPeer < &server->peers[server->peerCount];
				++currentPeer)
			{
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
					continue;
				if (currentPeer->address.host == peer->address.host)
					count++;
			}

			event.peer->data = new PlayerInfo;
			/* Get the string ip from peer */
			char clientConnection[16];
			enet_address_get_host_ip(&peer->address, clientConnection, 16);
			((PlayerInfo*)(peer->data))->charIP = clientConnection;
			if (count > 3)
			{
				packet::consolemessage(peer, "`rToo many accounts are logged on from this IP. Log off one account before playing please.``");
				enet_peer_disconnect_later(peer, 0);
			}
			else {
				sendData(peer, 1, 0, 0);
			}


			continue;
		}
		case ENET_EVENT_TYPE_RECEIVE:
		{
			
			/*if (((PlayerInfo*)(peer->data))->isUpdating)
			{
				cout << "packet drop" << endl;
				continue;
			}*/

			int messageType = GetMessageTypeFromPacket(event.packet);

			WorldInfo* world = getPlyersWorld(peer);
			switch (messageType) {
			case 2:
			{
				//cout << GetTextPointerFromPacket(event.packet) << endl;
				string cch = GetTextPointerFromPacket(event.packet);
				string str = cch.substr(cch.find("text|") + 5, cch.length() - cch.find("text|") - 1);
				
				if (cch.find("action|trade_started\n") == 0) {
					cout << "trade started\n";
				}
				//if (cch.find("action|trade_started\n"))
				if (cch.find("action|wrench") == 0) {
					std::stringstream ss(cch);
					std::string to;
					int id = -1;
					while (std::getline(ss, to, '\n')) {
						vector<string> infoDat = explode("|", to);
						if(infoDat.size() < 3) continue;
						if (infoDat[1] == "netid") {
							id = atoi(infoDat[2].c_str());
						}

					}
					if (id < 0) continue; //not found
					int worldcount = 0;
					ENetPeer * currentPeer;
					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							worldcount + 1;
							//Taking away one from the count to account for the currentplayer. :)
							worldcount - 1;

						}
					}
					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;

						if (isHere(peer, currentPeer)) {
							//wrenching info
							if (((PlayerInfo*)(currentPeer->data))->netID == id) {
								((PlayerInfo*)(peer->data))->lastInfo = ((PlayerInfo*)(currentPeer->data))->rawName;
								((PlayerInfo*)(peer->data))->lastInfoname = ((PlayerInfo*)(currentPeer->data))->tankIDName;
								string x;
								string x1;
								string x2;
								string x3;
								string name = ((PlayerInfo*)(currentPeer->data))->displayName;
								string rawname = ((PlayerInfo*)(currentPeer->data))->rawName;
								string crawname = ((PlayerInfo*)(currentPeer->data))->displayName;
								string growIP = ((PlayerInfo*)(currentPeer->data))->charIP;
								string level = std::to_string(((PlayerInfo*)(currentPeer->data))->level);
								if (((PlayerInfo*)(peer->data))->canDoubleJump == true) {
									x = "\nadd_label_with_icon|small|`wDouble Jump``|left|156|";
								}
								if (((PlayerInfo*)(peer->data))->canWalkInBlocks == true) {
									x1 = "|\nadd_label_with_icon|small|`wGhost in The Shell``|left|290|";
								}
								if (((PlayerInfo*)(peer->data))->isInvisible == true) {
									x2 = "|\nadd_label_with_icon|small|`wInvisibility``|left|3308|";
								}
								if (((PlayerInfo*)(peer->data))->isFrozenEffect == true) {
									x3 = "|\nadd_label_with_icon|small|`wFrozen Mod`` `o(`#" + to_string(((PlayerInfo*)(peer->data))->isFrozenEffectCounter) + "`o) `wseconds remaining|left|3308|";
								}
								if (((PlayerInfo*)(peer->data))->lastInfo == ((PlayerInfo*)(peer->data))->rawName) {
									//self
									packet::dialog(peer, "set_default_color`o\nadd_player_info|`2"+ crawname + "|" + std::to_string(((PlayerInfo*)(peer->data))->level) + "|" + std::to_string(((PlayerInfo*)(peer->data))->xp) +"|300|\nadd_spacer|small|\nadd_spacer|small|\nadd_button|renew_pvp_license|Get Card Battle License|noflags|0|0|\nadd_spacer|small|\nadd_button|title_edit|`$Title``|noflags|0|0|\nadd_inner_image_label_button|set_online_status|`$Set Online Status:``|game/tiles_page14.rttex|1.6|28|23|32|\nadd_button|notebook_edit|`$Notebook``|noflags|0|0|\nadd_button|goals|`$Goals & Quests``|noflags|0|0|\nadd_textbox|Complete 5 Life Goals and reach Level 5 to unlock Daily Bonuses!|left|\nadd_button|alist|`$Achivements (2`5/``169)``|noflags|0|0|\nadd_textbox|`wActive Effects:``|left|" + x1 + x + x3 + "|\nadd_spacer|small|\nadd_textbox|`oYou have `w16`` backpack slots.``|left|\nadd_textbox|`oCurrent world: `w"+ ((PlayerInfo*)(peer->data))->currentWorld + "`` (`w" + std::to_string(((PlayerInfo*)(peer->data))->x) + "``, `w" + std::to_string(((PlayerInfo*)(peer->data))->y) + "``) (`w" + std::to_string(worldcount) + "`` person)````|left|\nadd_textbox|`oYou are not yet a `2Supporter`` or `5Super Supporter``.``|left|\nadd_textbox|`oYou are standing on note \"G\".``|left|\nadd_spacer|small|\nadd_button|emojis|`$Growmojis``|noflags|0|0|\nadd_textbox|`oTotal time played is `wPLAYERHOURS``. This account was created `wDAYSSINCEACCOUNTCREATED``.|left|\nadd_spacer|small|\nend_dialog|popup||Continue|");
								}
								else {
									if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 499 || ((PlayerInfo*)(peer->data))->adminLevel > 499) {
										//packet::dialog(peer, "set_default_color|`o\nadd_label_with_icon|big|" + rawname + " `w(`2"+ level +"`w)|left|18|\nadd_spacer|small|\n\nadd_button|showInfo|`wTrade``|0|0|\n\nadd_button|showInfo|`1Freeze``|0|0|\n\nadd_button|showInfo|`1Punish/View``|0|0|\n\nadd_button|showInfo|`4Kick``|0|0|\n\nadd_button|showInfo|`5Pull``|0|0|\n\nadd_button|showInfo|`wAdd as Friend``|0|0|\n\nadd_quick_exit|\nend_dialog|player_info||Close|");
										if (getAdminLevel(((PlayerInfo*)(currentPeer->data))->rawName, ((PlayerInfo*)(currentPeer->data))->tankIDPass) > 900 || ((PlayerInfo*)(currentPeer->data))->adminLevel > 900) {
											//packet::dialog(peer, "set_default_color|`o\nadd_label_with_icon|big|" + rawname + " `w(`2?`w)|left|18|\nadd_spacer|small|\n\nadd_button|showInfo|`wTrade``|0|0|\n\nadd_button|showInfo|`1Freeze``|0|0|\n\nadd_button|showInfo|`1Punish/View``|0|0|\n\nadd_button|showInfo|`4Kick``|0|0|\n\nadd_button|showInfo|`5Pull``|0|0|\n\nadd_button|showInfo|`wAdd as Friend``|0|0|\n\nadd_quick_exit|\nend_dialog|player_info||Close|");
											packet::dialog(peer, "set_default_color|`o|\nadd_label_with_icon|big|`w" + name + " ``(`2" + level + "``)``|left|18|\nadd_spacer|small|\nadd_button|freeze|`1Freeze``|0|0|\n\nadd_button|showInfo|`1Punish/View``|0|0||\nadd_button|trade|`wTrade``|noflags|0|0||\nadd_textbox|(No Battle Leash equipped)|left||\nadd_textbox|You need a valid license to battle!|left||\nadd_button|friend_add|`wAdd as friend``|noflags|0|0||\nadd_button|ignore_player|`wIgnore Player``|noflags|0|0||\nadd_button|report_player|`wReport Player``|noflags|0|0||\nadd_spacer|small||\nend_dialog|popup||Continue||\nadd_quick_exit|");
										}
										else if (getAdminLevel(((PlayerInfo*)(currentPeer->data))->rawName, ((PlayerInfo*)(currentPeer->data))->tankIDPass) < 899) {
											//packet::dialog(peer, "set_default_color|`o\nadd_label_with_icon|big|" + rawname + " `w(`2" + level + "`w)|left|18|\nadd_spacer|small|\n\nadd_button|showInfo|`wTrade``|0|0|\n\nadd_button|showInfo|`1Freeze``|0|0|\n\nadd_button|showInfo|`1Punish/View``|0|0|\n\nadd_button|showInfo|`4Kick``|0|0|\n\nadd_button|showInfo|`5Pull``|0|0|\n\nadd_button|showInfo|`wAdd as Friend``|0|0|\n\nadd_quick_exit|\nend_dialog|player_info||Close|");
											packet::dialog(peer, "set_default_color|`o|\nadd_label_with_icon|big|`w" + name + " ``(`2" + level + "``)``|left|18|\nadd_spacer|small|\nadd_button|freeze|`1Freeze``|0|0|\n\nadd_button|showInfo|`1Punish/View``|0|0||\nadd_button|trade|`wTrade``|noflags|0|0||\nadd_textbox|(No Battle Leash equipped)|left||\nadd_textbox|You need a valid license to battle!|left||\nadd_button|friend_add|`wAdd as friend``|noflags|0|0||\nadd_button|ignore_player|`wIgnore Player``|noflags|0|0||\nadd_button|report_player|`wReport Player``|noflags|0|0||\nadd_spacer|small||\nend_dialog|popup||Continue||\nadd_quick_exit|");
										}
									}
									else if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) < 1) {
										if (getAdminLevel(((PlayerInfo*)(currentPeer->data))->rawName, ((PlayerInfo*)(currentPeer->data))->tankIDPass) > 900 || ((PlayerInfo*)(currentPeer->data))->adminLevel > 900) {
											//packet::dialog(peer, "set_default_color|`o\nadd_label_with_icon|big|" + name + "`w(`2?`w)|left|18|\nadd_spacer|small|\n\nadd_quick_exit|\nend_dialog|player_info||Close|");
											packet::dialog(peer, "set_default_color|`o|\nadd_label_with_icon|big|`w" + name + " ``(`2" + level + "``)``|left|18|\nadd_spacer|small||\nadd_button|trade|`wTrade``|noflags|0|0||\nadd_textbox|(No Battle Leash equipped)|left||\nadd_textbox|You need a valid license to battle!|left||\nadd_button|friend_add|`wAdd as friend``|noflags|0|0||\nadd_button|ignore_player|`wIgnore Player``|noflags|0|0||\nadd_button|report_player|`wReport Player``|noflags|0|0||\nadd_spacer|small||\nend_dialog|popup||Continue||\nadd_quick_exit|");
										}
										else if (getAdminLevel(((PlayerInfo*)(currentPeer->data))->rawName, ((PlayerInfo*)(currentPeer->data))->tankIDPass) < 899) {
											//packet::dialog(peer, "set_default_color|`o\nadd_label_with_icon|big|" + name + "`w(`2" + level + "`w)|left|18|\nadd_spacer|small|\n\nadd_quick_exit|\nend_dialog|player_info||Close|");
											packet::dialog(peer, "set_default_color|`o|\nadd_label_with_icon|big|`w" + name + " ``(`2" + level + "``)``|left|18|\nadd_spacer|small||\nadd_button|trade|`wTrade``|noflags|0|0||\nadd_textbox|(No Battle Leash equipped)|left||\nadd_textbox|You need a valid license to battle!|left||\nadd_button|friend_add|`wAdd as friend``|noflags|0|0||\nadd_button|ignore_player|`wIgnore Player``|noflags|0|0||\nadd_button|report_player|`wReport Player``|noflags|0|0||\nadd_spacer|small||\nend_dialog|popup||Continue||\nadd_quick_exit|");
										}
									}
								}
								
								//packet::dialog(peer, "set_default_color|`o\nadd_label_with_icon|big|"+name+"|left|18|\nadd_spacer|small|\n\nadd_quick_exit|\nend_dialog|player_info||Close|");
							}

						}

					}
				}
				if (cch.find("action|setSkin") == 0) {
					if (!world) continue;
					std::stringstream ss(cch);
					std::string to;
					int id = -1;
					string color;
					while (std::getline(ss, to, '\n')) {
						vector<string> infoDat = explode("|", to);
						if (infoDat[0] == "color") color = infoDat[1];
						if (has_only_digits(color) == false) continue;
						id = atoi(color.c_str());
						if (color == "2190853119") {
							id = -2104114177;
						}
						else if (color == "2527912447") {
							id = -1767054849;
						}
						else if (color == "2864971775") {
							id = -1429995521;
						}
						else if (color == "3033464831") {
							id = -1261502465;
						}
						else if (color == "3370516479") {
							id = -924450817;
						}

					}
					((PlayerInfo*)(peer->data))->skinColor = id;
					sendClothes(peer);
				}
				if (cch.find("action|friends") == 0) {
					packet::dialog(peer, "set_default_color|`o|\nadd_label_with_icon|big| `wPlayer Portal`` |left|1366||\nadd_spacer|small||\nadd_button|socialportal|`wSocial Portal``|noflags|0|0||\nadd_button|missions|`wMarvelous Missions``|noflags|0|0||\nadd_button|gazette|`wGazette``|noflags|0|0||\nadd_button|communityhub|`wCommunity Hub``|noflags|0|0||\nadd_quick_exit||\nend_dialog|playerportal|OK||");
				}
				/*string buyHdrText = "action|buy\nitem|";
				if (cch.find(buyHdrText) == 0)
				{
							packet::storepurchaseresult(peer, "The store has not been added, please add it.");
				}*/
				if (cch.find("action|buy\nitem|locks") == 0) {
					
					//packet::storerequest(peer, "set_description_text|Welcome to the `2Growtopia Store``! Select the item you'd like more info on.`o. `wWant to get `5Supporter`` status? Any Gem purchase (or`57,000`` Gems earned with free tapjoy`` offers) will make you one. You'll get new skin colors, the `5Recycle`` tool to convert unwanted items into Gems, and more bonuses!|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||0|0|0|0||||-1|-1|||0|\nadd_tab_button|locks_menu|Locks and Stuff|interface/large/btn_shop.rttex||1|1|0|0||||-1|-1|||0|\nadd_tab_button|itempack_menu|Item packs|interface/large/btn_shop.rttex||0|3|0|0||||-1|-1|||0|\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||0|4|0|0||||-1|-1|||0|\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky? We offer alternatives within...|0|5|0|0||||-1|-1|||0|\nadd_tab_button|token_menu|Growtoken items|interface/large/btn_shop.rttex||0|2|0|0||||-1|-1|||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|1|\nadd_button|itemomonth|`oItem Of The Month``|interface/large/store_buttons/store_buttons16.rttex|April 2021:`` `9Grow X Rocket Wings``!<CR><CR>`2You Get:`` 1 `9Grow X Rocket Wings``.<CR><CR>`5Description:`` Take off on a mission to Mars, or wherever your imagination takes you! These rocket wings use the latest in renewable energy, are kind to the environment, and approved by Growtech for commercial space flight!|0|3|350000|0||interface/large/gui_store_button_overlays1.rttex|0|0||-1|-1||1||||||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|2|\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|0|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|3|\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|1|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|");
					packet::storerequest(peer, "set_description_text|`2Locks And Stuff!``  Select the item you'd like more info on, or BACK to go back.|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||0|0|0|0||||-1|-1|||0||\nadd_tab_button|locks_menu|Locks And Stuff|interface/large/btn_shop.rttex||1|1|0|0||||-1|-1|||0||\nadd_tab_button|itempack_menu|Item Packs|interface/large/btn_shop.rttex||0|3|0|0||||-1|-1|||0||\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||0|4|0|0||||-1|-1|||0||\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky?  We offer alternatives within...|0|5|0|0||||-1|-1|||0||\nadd_tab_button|token_menu|Growtoken Items|interface/large/btn_shop.rttex||0|2|0|0||||-1|-1|||0||\nadd_button|world_lock|`oWorld Lock``|interface/large/store_buttons/store_buttons.rttex|`2You Get:`` 1 World Lock.<CR><CR>`5Description:`` Become the undisputed ruler of your domain with one of these babies.  It works like a normal lock except it locks the `$entire world``!  Won't work on worlds that other people already have locks on. You can even add additional normal locks to give access to certain areas to friends. `5It's a perma-item, is never lost when destroyed.``  `wRecycles for 200 Gems.``|0|7|2000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|world_lock_10_pack|`oWorld Lock Pack``|interface/large/store_buttons/store_buttons18.rttex|`2You Get:`` 10 World Locks.<CR><CR>`5Description:`` 10-pack of World Locks. Become the undisputed ruler of up to TEN kingdoms with these babies. Each works like a normal lock except it locks the `$entire world``!  Won't work on worlds that other people already have locks on. You can even add additional normal locks to give access to certain areas to friends. `5It's a perma-item, is never lost when destroyed.`` `wEach recycles for 200 Gems.``|0|3|20000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|small_lock|`oSmall Lock``|interface/large/store_buttons/store_buttons.rttex|`2You Get:`` 1 Small Lock.<CR><CR>`5Description:`` Protect up to `$10`` tiles.  Can add friends to the lock so others can edit that area as well. `5It's a perma-item, is never lost when destroyed.``|1|3|50|0|||-1|-1||-1|-1||1||||||0||\nadd_button|big_lock|`oBig Lock``|interface/large/store_buttons/store_buttons.rttex|`2You Get:`` 1 Big Lock.<CR><CR>`5Description:`` Protect up to `$48`` tiles.  Can add friends to the lock so others can edit that area as well. `5It's a perma-item, is never lost when destroyed.``|1|1|200|0|||-1|-1||-1|-1||1||||||0||\nadd_button|huge_lock|`oHuge Lock``|interface/large/store_buttons/store_buttons.rttex|`2You Get:`` 1 Huge Lock.<CR><CR>`5Description:`` Protect up to `$200`` tiles.  Can add friends to the lock so others can edit that area as well. `5It's a perma-item, is never lost when destroyed.``|0|4|500|0|||-1|-1||-1|-1||1||||||0||\nadd_button|builders_lock|`oBuilder's Lock``|interface/large/store_buttons/store_buttons17.rttex|`2You Get:`` 1 Builders Lock.<CR><CR>`5Description:`` Protect up to `$200`` tiles. Wrench the lock to limit it - it can either only allow building, or only allow breaking! `5It's a perma-item, is never lost when destroyed.``|0|2|50000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|ectojuicer|`oEctoJuicer``|interface/large/store_buttons/store_buttons20.rttex|`2You Get:`` 1 EctoJuicer.<CR><CR>`5Description:`` Infuse your muscles with the unearthly might of the Other Side! This spectral potion gives you the strength to wring every last drop of ectoplasm from a defeated Boss Ghost, granting you an EXTRA Boss Goo after a successful banishing!|0|0|30000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|grow_spray|`o5-pack of Grow Spray Fertilizer``|interface/large/store_buttons/store_buttons.rttex|`2You Get:`` 5 Grow Spray Fertilizers.<CR><CR>`5Description:`` Why wait?!  Treat yourself to a `$5-pack`` of amazing `wGrow Spray Fertilizer`` by GrowTech Corp.  Each bottle instantly ages a tree by `$1 hour``.|0|6|200|0|||-1|-1||-1|-1||1||||||0||\nadd_button|deluxe_grow_spray|`oDeluxe Grow Spray``|interface/large/store_buttons/store_buttons11.rttex|`2You Get:`` 1 Deluxe Grow Spray.<CR><CR>`5Description:`` GrowTech's new `$Deluxe`` `wGrow Spray`` instantly ages a tree by `$24 hours`` per bottle! That's somewhere around 25 times as much as regular Grow Spray!|0|2|900|0|||-1|-1||-1|-1||1||||||0||\nadd_button|fish_flakes|`oFish Flakes``|interface/large/store_buttons/store_buttons18.rttex|`2You Get:`` 5 Fish Flakes.<CR><CR>`5Description:`` Every fish adores these tasty flakes! Give a pinch to your Training Fish and fill their scaly bellies with aquatic goodness! Take the guesswork out of finnicky feedings with a treat you know they'll love!|0|2|7500|0|||-1|-1||-1|-1||1||||||0||\nadd_button|fish_medicine|`oFish Medicine``|interface/large/store_buttons/store_buttons18.rttex|`2You Get:`` 1 Fish Medicine.<CR><CR>`5Description:`` Make a sick Training Fish bright and healthy with this healing potion. One dose is enough to make even the sickest fish all better!|0|0|1500|0|||-1|-1||-1|-1||1||||||0||\nadd_button|fish_reviver|`oFish Reviver``|interface/large/store_buttons/store_buttons18.rttex|`2You Get:`` 1 `#Rare Fish Reviver``.<CR><CR>`5Description:`` Resurrect a dead Training Fish with a revivifying zap from this `#Rare`` Fish Reviver! One dose is enough to reach beyond the veil and bring a fish back from the dead! Comes with a 100% zombie-free guarantee!|0|1|5000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|signal_jammer|`oSignal Jammer``|interface/large/store_buttons/store_buttons.rttex|`2You Get:`` 1 Signal Jammer.<CR><CR>`5Description:`` Get off the grid! Install a `$Signal Jammer``! A single punch will cause it to whir to life, tireless hiding your world and its population from pesky snoopers - only those who know the world name will be able to enter. `5It's a perma-item, is never lost when destroyed.``|1|6|2000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|zombie_jammer|`oZombie Jammer``|interface/large/store_buttons/store_buttons7.rttex|`2You Get:`` 1 Zombie Jammer.<CR><CR>`5Description:`` Got a parkour or race that you don't want slowed down? Turn this on and nobody can be infected by zombie bites in your world. It does not prevent direct infection by the g-Virus itself though. `5It's a perma-item, is never lost when destroyed.``|0|5|15000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|punch_jammer|`oPunch Jammer``|interface/large/store_buttons/store_buttons7.rttex|`2You Get:`` 1 Punch Jammer.<CR><CR>`5Description:`` Tired of getting bashed around? Set up a Punch Jammer in your world, and people won't be able to punch each other! Can be turned on and off as needed. `5It's a perma-item, is never lost when destroyed.``|0|4|15000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|rename|`oBirth Certificate``|interface/large/store_buttons/store_buttons7.rttex|`2You Get:`` 1 Birth Certificate.<CR><CR>`5Description:`` Tired of being who you are? By forging a new birth certificate, you can change your GrowID! The Birth Certificate will be consumed when used. This item only works if you have a GrowID, and you can only use one every 60 days, so you're not confusing everybody.|0|6|20000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|change_addr|`oChange of Address``|interface/large/store_buttons/store_buttons12.rttex|`2You Get:`` 1 Change of Address.<CR><CR>`5Description:`` Don't like the name of your world? You can use up one of these to trade your world's name with the name of any other world that you own. You must have a `2World Lock`` in both worlds. Go lock up that empty world with the new name you want and swap away!|0|6|20000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|door_mover|`oDoor Mover``|interface/large/store_buttons/store_buttons8.rttex|`2You Get:`` 1 Door Mover.<CR><CR>`5Description:`` Unsatisfied with your world's layout?  This one-use device can be used to move the White Door to any new location in your world, provided there are 2 empty spaces for it to fit in. Disappears when used. `2Only usable on a world you have World Locked.``|0|6|5000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|vending_machine|`oVending Machine``|interface/large/store_buttons/store_buttons13.rttex|`2You Get:`` 1 Vending Machine.<CR><CR>`5Description:`` Tired of interacting with human beings? Try a Vending Machine! You can put a stack of items inside it, set a price in World Locks, and people can buy from the machine while you sit back and rake in the profits! `5It's a perma-item, is never lost when destroyed, and it is not available any other way.``|0|6|8000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|digi_vend|`oDigiVend Machine``|interface/large/store_buttons/store_buttons29.rttex|`2You Get:`` 1 DigiVend Machine.<CR><CR>`5Description:`` Get with the times and go digital! This wired vending machine can connect its contents to Vending Hubs AND the multiversal economy, providing a unified shopping experience along with price checks to help you sell your goods! All that, and still no human-related hassle! Use your wrench on this to stock it with an item and set a price in World Locks. Other players will be able to buy from it! Only works in World-Locked worlds.|0|2|12000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|checkout_counter|`oVending Hub - Checkout Counter``|interface/large/store_buttons/store_buttons29.rttex|`2You Get:`` 1 Vending Hub.<CR><CR>`5Description:`` Your one-stop shop! This vending hub will collect and display (and let shoppers buy) the contents of ALL DigiVends in its row or column (wrench it to set which the direction)! Wow! Now that's a shopping experience we can all enjoy! Note: Only works in World-Locked worlds.|0|3|50000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|guild_name_changer|`oGuild Name Changer``|interface/large/store_buttons/store_buttons23.rttex|`2You Get:`` 1 Guild Name Changer.<CR><CR>`5Description:`` Fancy a change? Bored of your guild name or made a mistake when creating it? Fear not, you can use up one of these to change your `2Guild's name``! The usual name checks will be initiated to check if your new guild name is valid. `4Only usable by the guild leader!``|0|6|100000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|safe_vault|`oSafe Vault``|interface/large/store_buttons/store_buttons27.rttex|`2You Get:`` 1 Safe Vault.<CR><CR>`5Description:`` Protect your items with a password with GrowTech's new Safe Vault!|0|4|10000|0|||-1|-1||-1|-1||1|||||");
				}
				if (cch.find("action|buy\nitem|oldlocksstuff") == 0) {
					packet::storerequest(peer, "set_description_text|Welcome to the `2Growtopia Store``! Select the item you'd like more info on.`o `wWant to get `5Supporter`` status? Any Gem purchase (or `57,000`` Gems earned with free `5Tapjoy`` offers) will make you one. You'll get new skin colors, the `5Recycle`` tool to convert unwanted items into Gems, and more bonuses!||\nadd_button|world_lock|`oWorld Lock``|interface/large/store_buttons/store_buttons.rttex|`2You Get:`` 1 World Lock.<CR><CR>`5Description:`` Become the undisputed ruler of your domain with one of these babies.  It works like a normal lock except it locks the `$entire world``!  Won't work on worlds that other people already have locks on. You can even add additional normal locks to give access to certain areas to friends. `5It's a perma-item, is never lost when destroyed.``  `wRecycles for 200 Gems.``|0|2||0|||-1|-1||-1|-1||1||||||0||\nadd_button|dungeon_pack|`oDungeon Pack``|interface/large/store_buttons/store_buttons4.rttex|`2You Get:`` 20 Grimstone, 20 Blackrock Wall, 20 Iron Bars, 3 Jail Doors, 3 Skeletons, 1 Headsman's Axe, 1 Worthless Rags. 5 Torches and a `#Rare Iron Mask!``.<CR><CR>`5Description:`` Lock up your enemies in a dank dungeon! Of course they can still leave whenever they want. But they won't want to, because it looks so cool! Iron Mask muffles your speech!|0|7|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|music_pack|`oComposer's Pack``|interface/large/store_buttons/store_buttons3.rttex|`2You Get:`` 20 Sheet Music: Blank, 20 Sheet Music: Piano Note, 20 Sheet Music: Bass Note, 20 Sheet Music Drums, 5 Sheet Music: Sharp Piano, 5 Sheet Music: Flat Piano, 5 Sheet Music: Flat Bass and 5 Sheet Music: Sharp Bass .<CR><CR>`5Description:`` With these handy blocks, you'll be able to compose your own music, using your World-Locked world as a sheet of music. Requires a World Lock (sold separately!).|0|3||0|||-1|-1||-1|-1||1||||||0||\nadd_button|farm_pack|`oFarm Pack``|interface/large/store_buttons/store_buttons5.rttex|`2You Get:`` 1 Cow, 1 Chicken, 10 Wheat, 10 Barn Block, 10 Red Wood Walls, 1 Barn Door, 1 Straw Hat, 1 Overalls, 1 Pitchfork, 1 Farmgirl Hair, 1 `#Rare`` `2Dear John Tractor``.<CR><CR>`5Description:`` Put the `2Grow`` in Growtopia with this pack, including a Cow you can milk, a Chicken that lays eggs and a farmer's outfit. Best of all? You get a `#Rare`` `2Dear John Tractor`` you can ride that will mow down trees!|0|4||0|||-1|-1||-1|-1||1||||||0||\nadd_button|school_pack|`oEducation Pack``|interface/large/store_buttons/store_buttons4.rttex|`2You Get:`` 10 ChalkBoards, 3 School Desks, 20 Red Bricks, 1 Bulletin Board, 10 Pencils, 1 Growtopia Lunchbox, 1 Grey Hair Bun, 1 Apple and 1 Random School Uniform Item.<CR><CR>`5Description:`` If you want to build a school in Growtopia, here's what you need!|0|6||0|||-1|-1||-1|-1||1||||||0||\nadd_button|home_pack|`oHome Pack``|interface/large/store_buttons/store_buttons9.rttex|`2You Get:`` 1 Television, 4 Couches, 2 Curtains, 1 Wall Clock, 1 Microwave, 1 Meaty Apron, 1 Ducky Pants, 1 Ducky top and 1 Eggs Benedict.<CR><CR>`5Description:`` Welcome home to Growtopia! Decorate with a Television, Window Curtains, Couches, a `#Rare`` Wall Clock that actually tells time, and a Microwave to cook in. Then dress up in a Meaty Apron and Ducky Pajamas to sit down and eat Eggs Benedict, which increases the amount of XP you earn!|0|0||0|||-1|-1||-1|-1||1||||||0|");

				}
				string buyhtext = "action|buy\nitem|main";
				if (cch.find(buyhtext) == 0) {
					//packet::storerequest(peer, "set_description_text|Welcome to the `2Growtopia Store``! Select the item you'd like more info on.`o. `wWant to get `5Supporter`` status? Any Gem purchase (or`57,000`` Gems earned with free tapjoy`` offers) will make you one. You'll get new skin colors, the `5Recycle`` tool to convert unwanted items into Gems, and more bonuses!|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||1|0|0|0||||-1|-1|||0|\nadd_tab_button|locks_menu|Locks and Stuff|interface/large/btn_shop.rttex||0|1|0|0||||-1|-1|||0|\nadd_tab_button|itempack_menu|Item packs|interface/large/btn_shop.rttex||0|3|0|0||||-1|-1|||0|\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||0|4|0|0||||-1|-1|||0|\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky? We offer alternatives within...|0|5|0|0||||-1|-1|||0|\nadd_tab_button|token_menu|Growtoken items|interface/large/btn_shop.rttex||0|2|0|0||||-1|-1|||0|\nadd_banner|interface/large/gui_shop_winter_seals.rttex|bannerlayout|OPENDIALOG|grow4goodcommunity|\nadd_button|rt_grope_eventpass_bundle01|Grow4Good Pass|interface/large/store_buttons/store_buttons35.rttex|https://growtopiagame.com|1|1|0||||-1|-1|`2You Get:`` Grow4Good Pass, and 350k gems. 1 Imperial Starship Blast, 3 Startopia Supply Crates, 3 Surgery Supply Crates, 3 Fishing Supply Crates, 3 Cooking Supply Crates, 3 Superhero Supply Crates instantly.<CR><CR>`5Description:`` Grow4Good Pass gives access to earn exclusive rewards including the One Winged Angel when you progress in the Grow4Good Personal Contributions as well as everything you need to enjoy all the activites during the event. <CR><CR>`210% of the revenue from all `5Grow4Good Passess`` sold throught the event will be donated to Operation Smile!``|1||||||0|\nadd_button|grow4good_personal|`oGrow4Good Personal Points|interface/large/store_buttons/store_buttons35.rttex|OPENDIALOG|&grow4goodpersonal|0|2|0|0|||-1|-1||-1|-1||1||||||0|\nadd_button|rt_grope_cosmetic_bundle01|`oOne Winged Angel``|interface/large/store_buttons/store_buttons35.rttex|https://growtopiagame.com|1|0|0||||-1|-1||-1|-1|`2You Get:`` 1 One Winged Angel Item.<CR><CR>`5Description:`` This single wing is a symbol for all those who strive to overcome adversity, no matter what the odds. To avoid refund fraud, this item has been made untradeable until August 25th, 2021.|1||||||0||\nadd_button|wavey_arm_guy|`oWaving Inflateable Arm Guy|interface/large/shop_buttons/shop_buttons35.rttex|`2You Get:`` 1 Waving Inflateable Arm Guy.<CR><CR>`5Description: ``Show your support for the Grow4Good Drive when you buy this incredible Waving Inflateable Arm Guy! Advertise how cool your world is with this cute guy waiting to greet everyone that enters.|1|2|50000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|itemomonth|`oItem Of The Month``|interface/large/store_buttons/store_buttons16.rttex|April 2021:`` `9Grow X Rocket Wings``!<CR><CR>`2You Get:`` 1 `9Grow X Rocket Wings``.<CR><CR>`5Description:`` Take off on a mission to Mars, or wherever your imagination takes you! These rocket wings use the latest in renewable energy, are kind to the environment, and approved by Growtech for commercial space flight!|0|3|350000|0||interface/large/gui_store_button_overlays1.rttex|0|0||-1|-1||1||||||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|2|\nadd_button|gems_glory|Road To Glory|interface/large/store_buttons/store_buttons30.rttex|https://growtopiagame.com|0|0|0|||interface/large/gui_store_buttons_overlays1.rttex|0|0|/interface/large/gui_shop_buybanner.rttex|1|0|`2You Get:`` Road To Glory and 100k Gems Instantly.<CR>`5Description:`` Earn Gem rewards when you level up. Every 10 levels you will get additional Gem rewards up until level 50! Claim all rewards instantly if you are over Level 50!! 1.6m Gems in total! |1||||||0||\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|1|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|");
					packet::storerequest(peer, "set_description_text|Welcome to the `2Growtopia Store``! Select the item you'd like more info on.`o `wWant to get `5Supporter`` status? Any Gem purchase (or `57,000`` Gems earned with free `5Tapjoy`` offers) will make you one. You'll get new skin colors, the `5Recycle`` tool to convert unwanted items into Gems, and more bonuses!|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||1|0|0|0||||-1|-1|||0||\nadd_tab_button|locks_menu|Locks And Stuff|interface/large/btn_shop.rttex||0|1|0|0||||-1|-1|||0||\nadd_tab_button|itempack_menu|Item Packs|interface/large/btn_shop.rttex||0|3|0|0||||-1|-1|||0||\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||0|4|0|0||||-1|-1|||0||\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky?  We offer alternatives within...|0|5|0|0||||-1|-1|||0||\nadd_tab_button|token_menu|Growtoken Items|interface/large/btn_shop.rttex||0|2|0|0||||-1|-1|||0||\nadd_image_button|banner|interface/large/gui_shop_winter_seals.rttex|bannerlayout|OPENDIALOG|grow4goodcomunity||\nadd_button|rt_grope_eventpass_bundle01|Grow4Good Pass|interface/large/store_buttons/store_buttons35.rttex|https://growtopiagame.com|1|1|0||||-1|-1||-1|-1|`2You Get:`` Grow4Good Pass, and 350k Gems. 1 Imperial Starship Blast, 3 Startopia Supply Crates, 3 Surgery Supply Crates, 3 Fishing Supply Crates, 3 Cooking Supply Crates, 3 Superhero Supply Crates Instantly.<CR><CR>`5Description:`` Grow4Good Pass gives access to earn exclusive rewards incuding the One Winged Angel when you progress in the Grow4Good Personal Contributions as well as everything you need to enjoy all the activities during the event.<CR><CR>`210% of the revenue from all `5Grow4Good Passes`` sold throughout the event will be donated to Operation Smile!``|1||||||0||\nadd_button|grow4good_personal|`oGrow4Good Personal Points|interface/large/store_buttons/store_buttons35.rttex|OPENDIALOG&grow4goodpersonal|0|2|0|0|||-1|-1||-1|-1||1||||||0||\nadd_button|rt_grope_cosmetic_bundle01|`oOne Winged Angel``|interface/large/store_buttons/store_buttons35.rttex|https://growtopiagame.com|1|0|0||||-1|-1||-1|-1|`2You Get:`` 1 One Winged Angel Item.<CR><CR>`5Description:`` This single wing is a symbol for all those who strive to overcome adversity, no matter what the odds. To avoid refund fraud, this item has been made untradeable until August 25th, 2021.|1||||||0||\nadd_button|wavey_arm_guy|`oWaving Inflatable Arm Guy|interface/large/store_buttons/store_buttons35.rttex|`2You Get:`` 1 Waving Inflatable Arm Guy.<CR><CR>`5Description:`` Show your support for the Grow4Good Drive when you buy this incredible Waving Inflatable Arm Guy! Advertise just how cool your world is with this cute guy waiting to greet everyone that enters.|1|2|50000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|gross_mixers|`oBag of Mutant Mixers``|interface/large/store_buttons/store_buttons27.rttex|`2You Get:`` 10 Rotting Sea Goo, 10 Space Poop, 10 Burnt Slime, 10 Putrid Gas and 10 Strawberry Slime Blocks and 1 Random Mutant Mamma's Cooking Recipe.<CR><CR>`5Description:`` A bag of `2Mutant Mixers`` to make deliciously mutated ingredients and one of `5Mutant Mumma's`` very own mutated recipes to cook with them! You also get some slippery `2Strawberry Slime Blocks``.|0|0|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|itemomonth|`oItem Of The Month``|interface/large/store_buttons/store_buttons16.rttex|April 2021:`` `9Grow X Rocket Wings``!<CR><CR>`2You Get:`` 1 `9Grow X Rocket Wings``.<CR><CR>`5Description:`` Take off on a mission to Mars, or wherever your imagination takes you! These rocket wings use the latest in renewable energy, are kind to the environment, and approved by Growtech for commercial space flight!|0|3|350000|0||interface/large/gui_store_button_overlays1.rttex|0|0||-1|-1||1||||||0||\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|2||\nadd_button|gems_glory|Road To Glory|interface/large/store_buttons/store_buttons30.rttex|https://growtopiagame.com|0|0|0|||interface/large/gui_store_button_overlays1.rttex|0|0|/interface/large/gui_shop_buybanner.rttex|1|0|`2You Get:`` Road To Glory and 100k Gems Instantly.<CR>`5Description:`` Earn Gem rewards when you level up. Every 10 levels you will get additional Gem rewards up to Level 50! Claim all rewards instantly if you are over level 50!! 1.6M Gems in total!! |1||||||0||\nadd_button|gems_bundle05|Gem Bounty|interface/large/store_buttons/store_buttons34.rttex|https://growtopiagame.com|0|6|0||||-1|-1||-1|-1|`2You Get:`` 900,000 Gems, 6 Growtokens and 3 Megaphones.<CR><CR>`5Description:`` Get a plethora of gems to add to your wealth.|1||||||0||\nadd_button|gems_rain|It's Rainin' Gems|interface/large/store_buttons/store_buttons.rttex|https://growtopiagame.com|1|5|0||||-1|-1||-1|-1|`2You Get:`` 290,000 Gems, 2 Growtokens and 1 Megaphone.<CR><CR>`5Description:`` All the gems you could ever want and more plus 2 Growtokens and a Megaphone to tell the whole world about it.|1||||||0||\nadd_button|gems_fountain|Gem Fountain|interface/large/store_buttons/store_buttons2.rttex|https://growtopiagame.com|0|2|0||||-1|-1||-1|-1|`2You Get:`` 130,000 Gems and 1 Growtoken.<CR><CR>`5Description:`` Get a pile of gems to shop to your hearts desire and 1 Growtoken.|1||||||0||\nadd_button|gems_chest|Chest o' Gems|interface/large/store_buttons/store_buttons.rttex|https://growtopiagame.com|0|5|0||||-1|-1||-1|-1|`2You Get:`` 45,000 Gems.<CR><CR>`5Description:`` Get a chest containing gems.|1||||||0||\nadd_button|gems_bag|Bag o' Gems|interface/large/store_buttons/store_buttons.rttex|https://growtopiagame.com|1|0|0||||-1|-1||-1|-1|`2You Get:`` 20,000 Gems.<CR><CR>`5Description:`` Get a small bag of gems.|1||||||0||\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|3||\nadd_button|365d|`o1-Year Subscription Token``|interface/large/store_buttons/store_buttons28.rttex|https://growtopiagame.com|0|2|0||||-1|-1||-1|-1|`2You Get:`` 1x 1-Year Subscription Token and 25 Growtokens.<CR><CR>`5Description:`` One full year of special treatment AND 25 Growtokens upfront! You'll get 70 season tokens (as long as there's a seasonal clash running), and 4000 gems every day and a chance of doubling any XP earned, growtime reduction on all seeds planted and Exclusive Skins!|1||||||0||\nadd_button|30d|`o30-Day Subscription Token``|interface/large/store_buttons/store_buttons28.rttex|https://growtopiagame.com|0|3|0||||-1|-1||-1|-1|`2You Get:`` 1x 30-Day Free Subscription Token and 2 Growtokens.<CR><CR>`5Description:`` 30 full days of special treatment AND 2 Growtokens upfront! You'll get 70 season tokens (as long as there's a seasonal clash running), and 4000 gems every day and a chance of doubling any XP earned, growtime reduction on all seeds planted and Exclusive Skins!|1||||||0|");
				}
				string buyitext = "action|buy\nitem|itempack";
				if (cch.find(buyitext) == 0) {
					//packet::storerequest(peer, "set_description_text|Welcome to the `2Growtopia Store``! Select the item you'd like more info on.`o. `wWant to get `5Supporter`` status? Any Gem purchase (or`57,000`` Gems earned with free tapjoy`` offers) will make you one. You'll get new skin colors, the `5Recycle`` tool to convert unwanted items into Gems, and more bonuses!|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||0|0|0|0||||-1|-1|||0|\nadd_tab_button|locks_menu|Locks and Stuff|interface/large/btn_shop.rttex||0|1|0|0||||-1|-1|||0|\nadd_tab_button|itempack_menu|Item packs|interface/large/btn_shop.rttex||1|3|0|0||||-1|-1|||0|\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||0|4|0|0||||-1|-1|||0|\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky? We offer alternatives within...|0|5|0|0||||-1|-1|||0|\nadd_tab_button|token_menu|Growtoken items|interface/large/btn_shop.rttex||0|2|0|0||||-1|-1|||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|1|\nadd_button|itemomonth|`oItem Of The Month``|interface/large/store_buttons/store_buttons16.rttex|April 2021:`` `9Grow X Rocket Wings``!<CR><CR>`2You Get:`` 1 `9Grow X Rocket Wings``.<CR><CR>`5Description:`` Take off on a mission to Mars, or wherever your imagination takes you! These rocket wings use the latest in renewable energy, are kind to the environment, and approved by Growtech for commercial space flight!|0|3|350000|0||interface/large/gui_store_button_overlays1.rttex|0|0||-1|-1||1||||||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|2|\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|0|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|3|\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|1|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|");
					string bruh = "\nadd_button|gang_pack|`oGangland Style``|interface/large/store_buttons/store_buttons2.rttex|`2You Get:`` 1 Fedora, 1 Dames Fedora, 1 Pinstripe Suit with Pants, 1 Flapper Headband with Dress, 1 Cigar, 1 Tommy Gun, 1 Victola and 10 Art Deco Blocks .<CR><CR>`5Description:`` Step into the 1920's with a Complete Outfit, a Tommygun, a Victrola that plays jazz music, and 10 Art Deco Blocks. It's the whole package!|0|6|5000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|race_pack|`oRacing Action Pack``|interface/large/store_buttons/store_buttons2.rttex|`2You Get:`` 1 Racing Start Flag, 1 Racing End Flag, 2 Checkpoints, 2 Big Old Sideways Arrows, 1 Big Old Up Arrow, 1 Big Old Down Arrow, 1 WristBand, 1 HeadBand, 1 Sports Ball Jersey and 1 Air Robinsons.<CR><CR>`5Description:`` Get all you need to host races in your worlds! You'll win the races too, with new Air Robinsons that make you run faster!|0|7|3500|0|||-1|-1||-1|-1||1||||||0||\nadd_button|music_pack|`oComposer's Pack``|interface/large/store_buttons/store_buttons3.rttex|`2You Get:`` 20 Sheet Music: Blank, 20 Sheet Music: Piano Note, 20 Sheet Music: Bass Note, 20 Sheet Music Drums, 5 Sheet Music: Sharp Piano, 5 Sheet Music: Flat Piano, 5 Sheet Music: Flat Bass and 5 Sheet Music: Sharp Bass .<CR><CR>`5Description:`` With these handy blocks, you'll be able to compose your own music, using your World-Locked world as a sheet of music. Requires a World Lock (sold separately!).|0|0|5000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|fantasy_pack|`oFantasy Pack``|interface/large/store_buttons/store_buttons3.rttex|`2You Get:`` 1 Mystical Wizard Hat Seed, 1 Wizards Robe, 1 Golden Sword, 1 Elvish Longbow, 10 Barrels, 3 Tavern Signs, 3 Treasure Chests and 3 Dragon Gates.<CR><CR>`5Description:`` Hear ye, hear ye! It's a pack of magical wonders!|0|6|5000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|school_pack|`oEducation Pack``|interface/large/store_buttons/store_buttons4.rttex|`2You Get:`` 10 ChalkBoards, 3 School Desks, 20 Red Bricks, 1 Bulletin Board, 10 Pencils, 1 Growtopia Lunchbox, 1 Grey Hair Bun, 1 Apple and 1 Random School Uniform Item.<CR><CR>`5Description:`` If you want to build a school in Growtopia, here's what you need!|0|0|5000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|dungeon_pack|`oDungeon Pack``|interface/large/store_buttons/store_buttons4.rttex|`2You Get:`` 20 Grimstone, 20 Blackrock Wall, 20 Iron Bars, 3 Jail Doors, 3 Skeletons, 1 Headsman's Axe, 1 Worthless Rags. 5 Torches and a `#Rare Iron Mask!``.<CR><CR>`5Description:`` Lock up your enemies in a dank dungeon! Of course they can still leave whenever they want. But they won't want to, because it looks so cool! Iron Mask muffles your speech!|0|7|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|zombie_pack|`oZombie Defense Pack``|interface/large/store_buttons/store_buttons4.rttex|`2You Get:`` 1 `#Rare Sawed-Off Shotgun``, 1 Combat Vest, 1 Zombie Stompin' Boots, 3 Traffic Barricades, 1 Military Radio, 1 Antidote, 3 Toxic Waste Barrels, 3 Biohazard Signs, 3 Tombstones and 1 `#Rare Deadly G-Virus``!.<CR><CR>`5Description:`` The zombie invasion has come! Protect yourself with all the esential zombie fighting gear and best of all, you get an Antidote to cure yourself! Also includes the deadly g-Virus itself to infect your friends with!|0|4|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|vegas_pack|`oVegas Pack``|interface/large/store_buttons/store_buttons4.rttex|`2You Get:`` 10 Neon Lights, 1 Card Block Seed, 1 `#Rare Pink Cadillac`` 4 Flipping Coins, 1 Dice Block, 1 Gamblers Visor, 1 Slot Machine, 1 Roulette Wheel and 1 Showgirl Hat, 1 Showgirl top and 1 Showgirl Leggins.<CR><CR>`5Description:`` What happens in Growtopia stays in Growtopia!|0|5|20000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|farm_pack|`oFarm Pack``|interface/large/store_buttons/store_buttons5.rttex|`2You Get:`` 1 Cow, 1 Chicken, 10 Wheat, 10 Barn Block, 10 Red Wood Walls, 1 Barn Door, 1 Straw Hat, 1 Overalls, 1 Pitchfork, 1 Farmgirl Hair, 1 `#Rare`` `2Dear John Tractor``.<CR><CR>`5Description:`` Put the `2Grow`` in Growtopia with this pack, including a Cow you can milk, a Chicken that lays eggs and a farmer's outfit. Best of all? You get a `#Rare`` `2Dear John Tractor`` you can ride that will mow down trees!|0|0|15000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|science_pack|`oMad Science Kit``|interface/large/store_buttons/store_buttons5.rttex|`2You Get:`` 1 Science Station, 1 Laboratory, 1 LabCoat, 1 Combover Hair, 1 Goggles, 5 Chemical R, 10 Chemical G, 5 Chemical Y, 5 Chemical B, 5 Chemical P and 1 `#Rare`` `2Death Ray``.<CR><CR>`5Description:`` It's SCIENCE! Defy the natural order with a Science Station that produces chemicals, a Laboratory in which to mix them and a full outfit to do so safely! You'll also get a starter pack of assorted chemicals. Mix them up! Special bonus: A `#Rare`` `2Death Ray`` to make your science truly mad!|0|3|5000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|city_pack|`oCity Pack``|interface/large/store_buttons/store_buttons6.rttex|`2You Get:`` 10 Sidewalks, 3 Street Signs, 3 Streetlamps, 10 Gothic Building tiles, 10 Tenement Building tiles, 10 Fire Escapes, 3 Gargoyles, 10 Hedges, 1 Blue Mailbox, 1 Fire Hydrant and A `#Rare`` `2ATM Machine``.<CR><CR>`5Description:`` Life in the big city is rough but a `#Rare`` `2ATM Machine`` that dishes out gems once a day is very nice!|0|0|8000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|west_pack|`oWild West Pack``|interface/large/store_buttons/store_buttons6.rttex|`2You Get:`` 1 Cowboy Hat, 1 Cowboy Boots, 1 War Paint, 1 Face Bandana, 1 Sheriff Vest, 1 Layer Cake Dress,  1 Corset, 1 Kansas Curls, 10 Western Building 1 Saloon Doors, 5 Western Banners, 1 Buffalo, 10 Rustic Fences, 1 Campfire and 1 Parasol.<CR><CR>`5Description:`` Yippee-kai-yay! This pack includes everything you need to have wild time in the wild west! The Campfire plays cowboy music, and the `#Parasol`` lets you drift down slowly. Special bonus: A `#Rare`` `2Six Shooter`` to blast criminals with!|0|2|8000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|astro_pack|`oAstro Pack``|interface/large/store_buttons/store_buttons6.rttex|`2You Get:`` 1 Astronaut Helmet, 1 Space Suit, 1 Space Pants, 1 Moon Boots, 1 Rocket Thruster, 1 Solar Panel, 6 Space Connectors, 1 Porthole, 1 Compu Panel, 1 Forcefield and 1 `#Rare`` `2Zorbnik DNA``.<CR><CR>`5Description:`` Boldly go where no Growtopian has gone before with an entire Astronaut outfit. As a special bonus, you can have this `#Rare`` `2Zorbnik DNA`` we found on a distant planet. It doesn't do anything by itself, but by trading with your friends, you can collect 10 of them, and then... well, who knows?|0|6|5000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|surgical_kit|`oSurgical Kit``|interface/large/store_buttons/store_buttons7.rttex|`2You Get:`` 1 `#Rare Heart Monitor``, 1 Hospital Bed, 1 Train-E Bot, 5 of each of the 13 different Surical Tools and 10 Med-a-Checks.<CR><CR>`5Description:`` Get all the tools you need to become Chief of Surgery at Growtopia General Hospital! `#Rare`` Heart Monitor that lets people know when you are online, Hospital Bed that lets you perform surgery on anybody laying (or standing) on it, Med-a-Checks to identify patients with maladies, The Train-E bot to practice on, and 5 each of the thirteen different Surgical Tools you'll need to do that surgery!|0|2|12000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|prehistoric_pack|`oPrehistoric Pack``|interface/large/store_buttons/store_buttons8.rttex|`2You Get:`` 1 Caveman Club, 1 Cave Woman Hair, 1 Caveman Hair, 1 Sabertooth Toga, 1 Fuzzy Bikini Top, 1 Fuzzy Bikni Bottom, 1 Cavewoman Outfit, 10 Cliffside, 5 Rock Platforms, 1 Cave Entrance, 3 Prehistoric Palms and 1 `#Rare Sabertooth Growtopian``.<CR><CR>`5Description:`` Travel way back in time with this pack, including full Caveman and Cavewoman outfits and `#Rare Sabertooth Growtopian`` (that's a mask of sorts). Unleash your inner monster!|0|0|5000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|shop_pack|`oShop Pack``|interface/large/store_buttons/store_buttons8.rttex|`2You Get:`` 4 Display Boxes, 1 For Sale Sign, 1 Gem Sign, 1 Exclamation Sign, 1 Shop Sign, 1 Open Sign, 1 Cash Register, 1 Mannequin and 1 Security Camera.<CR><CR>`5Description:`` Run a fancy shop with these new items! Advertise your wares with an Open/Closed Sign you can switch with a punch, a Cash Register, a Mannequin you can dress up to show off clothing, and a `#Rare`` Security Camera, which reports when people enter and take items!|0|7|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|home_pack|`oHome Pack``|interface/large/store_buttons/store_buttons9.rttex|`2You Get:`` 1 Television, 4 Couches, 2 Curtains, 1 Wall Clock, 1 Microwave, 1 Meaty Apron, 1 Ducky Pants, 1 Ducky top and 1 Eggs Benedict.<CR><CR>`5Description:`` Welcome home to Growtopia! Decorate with a Television, Window Curtains, Couches, a `#Rare`` Wall Clock that actually tells time, and a Microwave to cook in. Then dress up in a Meaty Apron and Ducky Pajamas to sit down and eat Eggs Benedict, which increases the amount of XP you earn!|0|6|5000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|cinema_pack|`oCinema Pack``|interface/large/store_buttons/store_buttons10.rttex|`2You Get:`` 1 ClapBoard, 1 Black Beret, 1 3D Glasses, 6 Theater Curtains, 6 Marquess Blocks, 1 Directors Chair, 4 Theater Seats, 6 Movie Screens, 1 Movie Camera and 1 `#Rare GHX Speaker``.<CR><CR>`5Description:`` It's movie time! Everything you need for the big screen experience including a `#Rare GHX Speaker`` that plays the score from Growtopia: The Movie.|0|2|6000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|adventure_pack|`oAdventure Pack``|interface/large/store_buttons/store_buttons10.rttex|`2You Get:`` 4 Gateways to Adventure, 4 Path Markers, 1 Lazy Cobra, 1 Adventure Brazier, 4 Adventure Barriers, 1 Rope, 1 Torch, 1 Key, 1 Golden Idol, 1 `#Rare Adventuring Mustache``, 1 Explorer's Ponytail and 1 Sling Bag .<CR><CR>`5Description:`` Join Dr. Exploro and her father (also technically Dr. Exploro) as they seek out adventure! You can make your own adventure maps with the tools in this pack.|0|7|20000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|rockin_pack|`oRockin' Pack``|interface/large/store_buttons/store_buttons11.rttex|`2You Get:`` 3 `#Rare Musical Instruments`` Including A Keytar, a Bass Guitar and Tambourine, 1 Starchild Make Up, 1 Rockin' Headband, 1 Leopard Leggings, 1 Shredded Ts-Shirt, 1 Drumkit, 6 Stage Supports, 6 Mega Rock Speakers and 6 Rock n' Roll Wallpaper.<CR><CR>`5Description:`` ROCK N' ROLL!!! Play live music in-game! We Formed a Band! Growtopia makes me want to rock out.|0|0|9999|0|||-1|-1||-1|-1||1||||||0|";
					string baha = "|\nadd_button|game_pack|`oGame Pack``|interface/large/store_buttons/store_buttons10.rttex|`2You Get:`` 1 `#Rare Game Generator``,  4 Game Blocks, 4 Game Flags, 4 Game Graves and 4 Game Goals.<CR><CR>`5Description:`` Growtopia's not all trading and socializing! Create games for your friends with the Game Pack (and a lot of elbow grease).|0|6|50000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|superhero|`oSuperhero Pack``|interface/large/store_buttons/store_buttons12.rttex|`2You Get:`` 1 Mask, 1 Shirt, 1 Boots, 1 Tights, 1 Cape, `#Rare Super Logos`` or `#Rare Utility Belt`` and 1 `2Phone Booth``.<CR><CR>`5Description:`` Battle the criminal element in Growtopia with a complete random superhero outfit including a cape that lets you double jump. Each of these items comes in one of six random colors. You also get one of 5 `#Rare`` Super Logos, which automatically match the color of any shirt you wear or a `#Rare`` Utility Belt... of course use the `2Phone Booth`` to change into your secret identity!|0|0|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|crime_wave|`oCrime Wave``|interface/large/store_buttons/store_buttons12.rttex|`2You Get:`` 5 Random Superpower Cards and 1 `#Rare Crime Wave``.<CR><CR>`5Description:`` Get powered up with random Superpower Cards, and what good would that be without a `#Rare`` Crime Wave to use them on? A Crime Wave is a one-use item that calls four villains to your world for you to battle. `6Beware:`` Villains only stick around for 24 hours once they appear.|0|5|5000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|fashion_pack|`oFashion Pack``|interface/large/store_buttons/store_buttons13.rttex|`2You Get:`` 3 Random Clothing Items, 3 Jade Blocks and 1 `#Rare Spotlight``.<CR><CR>`5Description:`` The hottest new looks for the season are here now with 3 random Fashion Clothing (dress, shoes, or purse), Jade Blocks to pose on, and a `#Rare`` Spotlight to shine on your fabulousness.|0|0|6000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|sportsball_pack|`oSportsball Pack``|interface/large/store_buttons/store_buttons13.rttex|`2You Get:`` 2 Basketball Hoops, 2 Sporty Goals, 5 Stadiums, 5 Crowded Stadiums, 10 Field Grass, 1 Football Helmet, 1 Growies Cap, 1 Ref's Jersey, 1 World Cup Jersey, 1 `#Rare Sports Item`` or `#Rare Growmoji!``.<CR><CR>`5Description:`` We like sports and we don't care who knows! This pack includes everything you need to get sporty! Use the Sports Items to launch Sportsballs at each other.|0|1|20000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|fishin_pack|`oFishin' Pack``|interface/large/store_buttons/store_buttons14.rttex|`2You Get:`` 1 Fishing Rod, 5 Wiggly Worms, 1 Hand Drill, 1 Nuclear Detonator,  1 `#Rare Tackle Box``, 10 Fish Tanks and 1 `#Rare Fish Tank Port`` .<CR><CR>`5Description:`` Relax and sit by the shore... this pack includes a Fishing Rod, Wiggly Worms for bait, Hand Drill, Nuclear Detonator, and a `#Rare`` Tackle Box which provides you with more free bait every two days, Fish Tanks, and a `#Rare`` Fish Tank Port to put the fish you catch into your fish tank!|0|0|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|fish_training_pack|`oFish Trainin' Pack``|interface/large/store_buttons/store_buttons17.rttex|`2You Get:`` 2 Fish Flakes, 2 Fish Medicine, AND 1 `#Rare Training Port``.<CR><CR>`5Description:`` Get ready to train your favorite fish! Use the Training Port to put a perfect fish into your fish tank for training!|0|7|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|firefighter|`oFirefighter Pack``|interface/large/store_buttons/store_buttons14.rttex|`2You Get:`` 1 Yellow Helmet, 1 Yellow Jacket, 1 Yellow Pants, 1 Firemans Boots, 1 Fire Hose, and 1 `#Rare Firehouse`` .<CR><CR>`5Description:`` Rescue Growtopians from the fire! Includes a full Yellow Firefighter Outfit, Fire Hose and a `#Rare Firehouse``, which will protect your own world from fires.|0|1|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|steampack|`oSteampack``|interface/large/store_buttons/store_buttons14.rttex|`2You Get:`` 10 Steam Tubes, 2 Steam Stompers, 2 Steam Organs, 2 Steam Vents, 2 Steam Valves and 1 `#Rare Steampunk Top Hat``.<CR><CR>`5Description:`` Steam! It's a wondrous new technology that lets you create paths of Steam Blocks, then jump on a Steam Stomper to launch a jet of steam through the path, triggering steam-powered devices. Build puzzles, songs, parkour challenges, and more!|0|6|20000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|paintbrush|`oPainter's Pack``|interface/large/store_buttons/store_buttons15.rttex|`2You Get:`` 1 `#Rare Paintbrush`` and 20 Random Colored Paint Buckets.<CR><CR>`5Description:`` Want to paint your world? This pack includes 20 buckets of random paint colors (may include Varnish, to clean up your messes)! You can paint any block in your world different colors to personalize it.|0|1|30000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|boo_pack|`oB.O.O. Training Pack``|interface/large/store_buttons/store_buttons15.rttex|`2You Get:`` 1 `#Rare Spectral Goggles``, 1 Neutron Gun, 1 Neutron Pack and 10 Ghost Jars <CR><CR>`5Description:`` It looks like Growtopia is under siege by ghosts! Well, the `9Battlers Of the Otherworldly`` are hiring! You'll have to earn your uniform, but this pack includes all the tools you need to actually capture ghosts! Including `#Rare`` Spectral Goggles (all the better to see them with!)and a Neutron Pack to corral the ghosts, of course 10 Ghost Jars to catch them in.|0|4|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|ghost_hunting|`oGhost Hunter's Pack``|interface/large/store_buttons/store_buttons19.rttex|`2You Get:`` At least 5 Ghost Jars and 1 Bonus Item.<CR><CR>`5Description:`` Essentials for Ghost Hunting! Guaranteed to have at least 5x Ghost Jars, plus one or more bonus items! Prizes can include: Neutron Focus Cores, Containment Field Power Nodes, EXTRA Ghost Jars, Ghost Traps, Spirit Boards, and maybe even a Dark Spirit Board!|0|7|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|paleo_kit|`oPaleontologist's Kit``|interface/large/store_buttons/store_buttons16.rttex|`2You Get:`` 5 Fossil Brushes, 1 Rock Hammer, 1 Rock Chisel, 1 Blue Hardhat and 1 `#Rare Fossil Prep Station``.<CR><CR>`5Description:`` If you want to dig up fossils, this is the kit for you! Includes everything you need! Use the prepstation to get your fossils ready for display.|0|0|20000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|chemsynth|`oChemsynth Pack``|interface/large/store_buttons/store_buttons16.rttex|`2You Get:`` 1 `#Rare Chemsynth Processor``, 10 Chemsynth Tanks and 1 Chemsynth Replicator, 1 Chemsynth Catalyst, 1 Chemsynth Solvent, 1 Chemsynth Centrifuge, 1 Chemsynth Stirrer.<CR><CR>`5Description:`` Tired of the lousy chemicals nature has to offer? Create new synthetic ones! With a `#Rare`` Chemsynth Processor, Chemsynth Tanks, and one each of the five Chemsynth tools, you can be whipping up Synthetic Chemicals in no time. `6Warning:`` Chemsynth solving is a pretty tricky puzzle, and it costs a whole bunch of the five basic chemicals (R, G, B, P, and Y) to complete.|0|4|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|eye_drops|`oEye Drop Pack``|interface/large/store_buttons/store_buttons17.rttex|`2You Get:`` 1 `#Rare Bathroom Mirror`` and 10 random Eye Drop Colors.<CR><CR>`5Description:`` Need a fresh new look?  This pack includes a 10 random Eye Drop Colors (may include Eye Cleaning Solution, to leave your eyes sparkly clean)!|0|6|30000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|robot_starter_pack|`oCyBlocks Starter Pack``|interface/large/store_buttons/store_buttons18.rttex|`2You Get:`` 1 `5Rare ShockBot`` and 10 random movement commands.<CR><CR>`5Description:`` CyBlocks Starter Pack includes one `5Rare`` ShockBot and 10 random movement commands to use with it. `5ShockBot`` is a perma-item, is never lost when destroyed.|0|6|5000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|robot_command_pack|`oCyBlocks Command Pack``|interface/large/store_buttons/store_buttons19.rttex|`2You Get:`` 10 Random CyBlock Commands.<CR><CR>`5Description:`` Grants 10 random CyBlock Commands to help control your CyBots!|0|2|2000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|robot_pack|`oCyBot Pack``|interface/large/store_buttons/store_buttons19.rttex|`2You Get:`` 1 `5Rare CyBot``!<CR><CR>`5Description:`` Grants one random `5Rare`` CyBot! Use CyBlock Commands to send these mechanical monsters into action! `5Note: Each CyBot is a perma-item, and will never be lost when destroyed.``|0|3|15000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|guild_chest_pack|`oGuild Chest Pack``|interface/large/store_buttons/store_buttons19.rttex|`2You Get:`` 10 Guild Chests.<CR><CR>`5Description:`` A 10-pack of Guild Chests! Loaded with guildy goodness - pop a chest open for a surprise item!|0|4|20000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|star_supplies|`oGalactic Goodies``|interface/large/store_buttons/store_buttons21.rttex|`2You Get:`` 60 Star Tools and 25 Star Fuel.<CR><CR>`5Description:`` Get all the Star Tools you need to boldly go where no Growtopian has gone! Use these to help you command a starship and seek victory in the Galactic Nexus! You'll get 5 each of the 12 Star Tools you'll need to complete missions and some bonus Star Fuel to help power a Starship!|0|0|15000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|contact_lenses|`oContact Lens Pack``|interface/large/store_buttons/store_buttons22.rttex|`2You Get:`` 10 Random Contact Lens Colors.<CR><CR>`5Description:`` Need a colorful new look? This pack includes 10 random Contact Lens colors (and may include Contact Lens Cleaning Solution, to return to your natural eye color)!|0|7|15000|0|||-1|-1||-1|-1||1||||||0|";
					packet::storerequest(peer, "set_description_text | `2Item Packs!``  Select the item you'd like more info on, or BACK to go back.|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||0|0|0|0||||-1|-1|||0||\nadd_tab_button|locks_menu|Locks And Stuff|interface/large/btn_shop.rttex||0|1|0|0||||-1|-1|||0||\nadd_tab_button|itempack_menu|Item Packs|interface/large/btn_shop.rttex||1|3|0|0||||-1|-1|||0||\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||0|4|0|0||||-1|-1|||0||\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky?  We offer alternatives within...|0|5|0|0||||-1|-1|||0||\nadd_tab_button|token_menu|Growtoken Items|interface/large/btn_shop.rttex||0|2|0|0||||-1|-1|||0||\nadd_button|5seed|`oSmall Seed Pack``|interface/large/store_buttons/store_buttons.rttex|`2You Get:`` 1 Small Seed Pack.<CR><CR>`5Description:`` Contains one Small Seed Pack. Open it for `$5`` randomly chosen seeds, including 1 rare seed! Who knows what you'll get ? !| 1 | 4 | 100 | 0|||-1 | -1 || -1 | -1 || 1||||||0 || \nadd_button | ssp_10_pack | `oSmall Seed Pack Collection``|interface / large / store_buttons / store_buttons18.rttex | `2You Get :`` 10 Small Seed Packs.<CR><CR>`5Description:`` Open each one for `$5`` randomly chosen seeds apiece, including 1 rare seed per pack!Who knows what you'll get?!|0|4|1000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|basic_splice|`oBasic Splicing Kit``|interface/large/store_buttons/store_buttons2.rttex|`2You Get:`` 10 Rock Seeds and 10 Random Seeds of Rarity 2.<CR><CR>`5Description:`` The basic seeds every farmer needs.|0|3|200|0|||-1|-1||-1|-1||1||||||0||\nadd_button|rare_seed|`oRare Seed Pack``|interface/large/store_buttons/store_buttons.rttex|`2You Get:`` 5 Randomly Chosen Rare Seeds.<CR><CR>`5Description:`` Expect some wondrous crops with these!|1|7|1000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|bountiful_seed_pack|`oBountiful Seed Pack``|interface/large/store_buttons/store_buttons28.rttex|`2You Get:`` 1 Bountiful Seed Pack.<CR><CR>`5Description:`` Contains `$5`` randomly chosen bountiful seeds, including 1 rare seed! Who knows what you'll get ? !| 0 | 4 | 1000 | 0|||-1 | -1 || -1 | -1 || 1||||||0 || \nadd_button | door_pack | `oDoor And Sign Hello Pack``|interface / large / store_buttons / store_buttons.rttex | `2You Get :`` 1 Door and 1 Sign.<CR><CR>`5Description:`` Own your very own door and sign!This pack comes with one of each.Leave cryptic messages and create a door that can open to, well, anywhere. | 0 | 3 | 15 | 0|||-1 | -1 || -1 | -1 || 1||||||0 || \nadd_button | clothes | `oClothes Pack``|interface / large / store_buttons / store_buttons2.rttex | `2You Get :`` 3 Randomly Wearble Items.<CR><CR>`5Description:`` Why not look the part ? Some may even have special powers... | 0 | 0 | 50 | 0|||-1 | -1 || -1 | -1 || 1||||||0 || \nadd_button | rare_clothes | `oRare Clothes Pack``|interface / large / store_buttons / store_buttons2.rttex | `2You Get :`` 3 Randomly Chosen Wearbale Items.<CR><CR>`5Description:`` Enjoy the garb of kings!Some may even have special powers... | 0 | 1 | 500 | 0|||-1 | -1 || -1 | -1 || 1||||||0 || " + bruh + baha);
				}
				string buybtext = "action|buy\nitem|bigitems";
				if (cch.find(buybtext) == 0) {
					//packet::storerequest(peer, "set_description_text|Welcome to the `2Growtopia Store``! Select the item you'd like more info on.`o. `wWant to get `5Supporter`` status? Any Gem purchase (or`57,000`` Gems earned with free tapjoy`` offers) will make you one. You'll get new skin colors, the `5Recycle`` tool to convert unwanted items into Gems, and more bonuses!|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||0|0|0|0||||-1|-1|||0|\nadd_tab_button|locks_menu|Locks and Stuff|interface/large/btn_shop.rttex||0|1|0|0||||-1|-1|||0|\nadd_tab_button|itempack_menu|Item packs|interface/large/btn_shop.rttex||0|3|0|0||||-1|-1|||0|\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||1|4|0|0||||-1|-1|||0|\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky? We offer alternatives within...|0|5|0|0||||-1|-1|||0|\nadd_tab_button|token_menu|Growtoken items|interface/large/btn_shop.rttex||0|2|0|0||||-1|-1|||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|1|\nadd_button|itemomonth|`oItem Of The Month``|interface/large/store_buttons/store_buttons16.rttex|April 2021:`` `9Grow X Rocket Wings``!<CR><CR>`2You Get:`` 1 `9Grow X Rocket Wings``.<CR><CR>`5Description:`` Take off on a mission to Mars, or wherever your imagination takes you! These rocket wings use the latest in renewable energy, are kind to the environment, and approved by Growtech for commercial space flight!|0|3|350000|0||interface/large/gui_store_button_overlays1.rttex|0|0||-1|-1||1||||||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|2|\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|0|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|3|\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|1|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|");
					packet::storerequest(peer, "set_description_text|`2Awesome Items!``  Select the item you'd like more info on, or BACK to go back.|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||0|0|0|0||||-1|-1|||0||\nadd_tab_button|locks_menu|Locks And Stuff|interface/large/btn_shop.rttex||0|1|0|0||||-1|-1|||0||\nadd_tab_button|itempack_menu|Item Packs|interface/large/btn_shop.rttex||0|3|0|0||||-1|-1|||0||\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||1|4|0|0||||-1|-1|||0||\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky?  We offer alternatives within...|0|5|0|0||||-1|-1|||0||\nadd_tab_button|token_menu|Growtoken Items|interface/large/btn_shop.rttex||0|2|0|0||||-1|-1|||0||\nadd_button|nyan_hat|`oTurtle Hat``|interface/large/store_buttons/store_buttons3.rttex|`2You Get:`` 1 Turtle Hat.<CR><CR>`5Description:`` It's the greatest hat ever. It bloops out bubbles as you run! `4Not available any other way!``|0|2|25000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|tiny_horsie|`oTiny Horsie``|interface/large/store_buttons/store_buttons3.rttex|`2You Get:`` 1 Tiny Horsie.<CR><CR>`5Description:`` Tired of wearing shoes? Wear a Tiny Horsie instead! Or possibly a large dachshund, we're not sure. Regardless, it lets you run around faster than normal, plus you're on a horse! `4Not available any other way!``|0|5|25000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|star_ship|`oPleiadian Star Ship``|interface/large/store_buttons/store_buttons4.rttex|`2You Get:`` 1 Pleiadian Star Ship.<CR><CR>`5Description:`` Float on, my brother. It's all groovy. This star ship can't fly, but you can still zoom around in it, leaving a trail of energy rings and moving at enhanced speed. Sponsored by Pleiadian. `4Not available any other way!``|0|3|25000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|dragon_hand|`oDragon Hand``|interface/large/store_buttons/store_buttons5.rttex|`2You Get:`` 1 Dragon Hand.<CR><CR>`5Description:`` Call forth the dragons of legend!  With the Dragon Hand, you will command your own pet dragon. Instead of punching blocks or players, you can order your dragon to incinerate them! In addition to just being awesome, this also does increased damage, and pushes other players farther. `4Not available any other way!``|0|1|50000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|corvette|`oLittle Red Corvette``|interface/large/store_buttons/store_buttons6.rttex|`2You Get:`` 1 Little Red Corvette.<CR><CR>`5Description:`` Cruise around the neighborhood in style with this sweet convertible. It moves at enhanced speed and leaves other Growtopians in your dust. `4Not available any other way!``|0|1|25000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|stick_horse|`oStick Horse``|interface/large/store_buttons/store_buttons6.rttex|`2You Get:`` 1 Stick Horse.<CR><CR>`5Description:`` Nobody looks cooler than a person bouncing along on a stick with a fake horse head attached. NOBODY. `4Not available any other way!``|0|3|25000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|ambulance|`oAmbulance``|interface/large/store_buttons/store_buttons7.rttex|`2You Get:`` 1 Ambulance.<CR><CR>`5Description:`` Rush to the scene of an accident while lawyers chase you in this speedy rescue vehicle. `4Not available any other way!``|0|3|25000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|raptor|`oRiding Raptor``|interface/large/store_buttons/store_buttons7.rttex|`2You Get:`` 1 Riding Raptor.<CR><CR>`5Description:`` Long thought to be extinct, it turns out that these dinosaurs are actually alive and easily tamed. And riding one lets you run around faster than normal! `4Not available any other way!``|0|7|25000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|owl|`oMid-Pacific Owl``|interface/large/store_buttons/store_buttons10.rttex|`2You Get:`` 1 Mid-Pacific Owl.<CR><CR>`5Description:`` This owl is a bit lazy - if you stop moving around, he'll land on your head and fall asleep. Dedicated to the students of the Mid-Pacific Institute. `4Not available any other way!``|0|1|30000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|unicorn|`oUnicorn Garland``|interface/large/store_buttons/store_buttons10.rttex|`2You Get:`` 1 Unicorn Garland.<CR><CR>`5Description:`` Prance about in the fields with your very own pet unicorn! It shoots `1R`2A`3I`4N`5B`6O`7W`8S``. `4Not available any other way!``|0|4|50000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|starboard|`oStarBoard``|interface/large/store_buttons/store_buttons11.rttex|`2You Get:`` 1 StarBoard.<CR><CR>`5Description:`` Hoverboards are here at last! Zoom around Growtopia on this brand new model, which is powered by fusion energy (that means stars spit out of the bottom). Moves faster than walking. Sponsored by Miwsky, Chudy, and Dawid. `4Not available any other way!``|0|1|30000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|motorcycle|`oGrowley Motorcycle``|interface/large/store_buttons/store_buttons11.rttex|`2You Get:`` 1 Growley Motorcycle.<CR><CR>`5Description:`` The coolest motorcycles available are Growley Dennisons. Get a sporty blue one today! It even moves faster than walking, which is pretty good for a motorcycle. `4Not available any other way!``|0|6|50000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|geiger|`oGeiger Counter``|interface/large/store_buttons/store_buttons12.rttex|`2You Get:`` 1 Geiger Counter.<CR><CR>`5Description:`` With this fantabulous device, you can detect radiation around you. It bleeps red, then yellow, then green as you get closer to the source. Who knows what you might find? `4Not available any other way!``|0|1|25000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|monkey_on_back|`oMonkey On Your Back``|interface/large/store_buttons/store_buttons13.rttex|`2You Get:`` 1 Monkey On Your Back.<CR><CR>`5Description:`` Most people work really hard to get rid of these, but hey, if you want one, it's available! `4But not available any other way!`` Sponsored by SweGamerHD's subscribers, Kizashi, and Inforced. `#Note: This is a neck item, not a back item. He's grabbing your neck!``|0|2|50000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|carrot_sword|`oCarrot Sword``|interface/large/store_buttons/store_buttons13.rttex|`2You Get:`` 1 Carrot Sword.<CR><CR>`5Description:`` Razor sharp, yet oddly tasty. This can carve bunny symbols into your foes! `4Not available any other way!`` Sponsored by MrMehMeh.|0|3|15000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|red_bicycle|`oRed Bicycle``|interface/large/store_buttons/store_buttons13.rttex|`2You Get:`` 1 Red Bicycle.<CR><CR>`5Description:`` It's the environmentally friendly way to get around! Ride this bicycle at high speed hither and zither throughout Growtopia. `4Not available any other way!``|0|5|30000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|fire_truck|`oFire Truck``|interface/large/store_buttons/store_buttons14.rttex|`2You Get:`` 1 Fire Truck.<CR><CR>`5Description:`` Race to the scene of the fire in this speedy vehicle! `4Not available any other way!``|0|2|50000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|pet_slime|`oPet Slime``|interface/large/store_buttons/store_buttons14.rttex|`2You Get:`` 1 Pet Slime.<CR><CR>`5Description:`` What could be better than a blob of greasy slime that follows you around? How about a blob of greasy slime that follows you around and spits corrosive acid, melting blocks more quickly than a normal punch? `4Not available any other way!``|0|4|100000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|silkworm|`oSilkworm``|interface/large/store_buttons/store_buttons14.rttex|`2You Get:`` 1 Silk Worm.<CR><CR>`5Description:`` It's the newest cuddly pet from Growtech Pharma! Thanks to genetic engineering, you can now raise your own giant mutant silkworm. They'll eat almost any food, but don't forget to give them water too! And if they get sick, you'll need to have some Antidotes on hand. `6Warning:`` `9Silkworms are living creatures. They will not live forever! Take good care of them, and enjoy them while you can.`` `4Not available any other way``.|0|7|7000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|antigravity_generator|`oAntigravity Generator``|interface/large/store_buttons/store_buttons17.rttex|`2You Get:`` 1 Antigravity Generator.<CR><CR>`5Description:`` Disables gravity in your world when activated! Well, it reduces gravity, and lets everybody jump as much as they want! `5It's a perma-item - never lost when destroyed! `4Not available any other way!````|0|3|450000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|dabstep_shoes|`oDabstep Low Top Sneakers``|interface/large/store_buttons/store_buttons21.rttex|`2You Get:`` 1 Dabstep Low Top Sneakers.<CR><CR>`5Description:`` Light up every footfall and move to a better beat with these dabulous shoes! When you're wearing these, the world is your dance floor! `4Not available any other way!``|0|2|30000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|building_blocks_machine|`oBuilding Blocks Machine``|interface/large/store_buttons/store_buttons26.rttex|`2You Get:`` 1 Building Blocks Machine.<CR><CR>`5Description:`` Eager to add some new building materials to your construction stockpile? Tired of collecting them from random worlds and weirdos? Well, pop this beauty in your world and it'll start cranking out awesome blocks in no time! Contains the `5RARE Creepy Baby Block and Digital Dirt`` amongst a heap of other new blocks! Careful, though - blocks don't just come from nothing, and this machine will eventually run out of power once it makes a bunch!|0|3|8000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|transmutation_device|`oTransmutabooth``|interface/large/store_buttons/store_buttons27.rttex|`2You Get:`` 1 Transmutabooth.<CR><CR>`5Description:`` Behold! A wondrous technological achievement from the innovative minds at GrowTech, the Transmutabooth allows you to merge clothing items, transferring the visual appearance of one onto another in the same slot! If you've ever wanted your Cyclopean Visor to look like Shades (while keeping its mod), now you can!|0|7|25000|0|||-1|-1||-1|-1||1||||||0|");
				}
				string buywtext = "action|buy\nitem|weather";
				if (cch.find(buywtext) == 0) {
					//packet::storerequest(peer, "set_description_text|Welcome to the `2Growtopia Store``! Select the item you'd like more info on.`o. `wWant to get `5Supporter`` status? Any Gem purchase (or`57,000`` Gems earned with free tapjoy`` offers) will make you one. You'll get new skin colors, the `5Recycle`` tool to convert unwanted items into Gems, and more bonuses!|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||0|0|0|0||||-1|-1|||0|\nadd_tab_button|locks_menu|Locks and Stuff|interface/large/btn_shop.rttex||0|1|0|0||||-1|-1|||0|\nadd_tab_button|itempack_menu|Item packs|interface/large/btn_shop.rttex||0|3|0|0||||-1|-1|||0|\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||0|4|0|0||||-1|-1|||0|\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky? We offer alternatives within...|1|5|0|0||||-1|-1|||0|\nadd_tab_button|token_menu|Growtoken items|interface/large/btn_shop.rttex||0|2|0|0||||-1|-1|||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|1|\nadd_button|itemomonth|`oItem Of The Month``|interface/large/store_buttons/store_buttons16.rttex|April 2021:`` `9Grow X Rocket Wings``!<CR><CR>`2You Get:`` 1 `9Grow X Rocket Wings``.<CR><CR>`5Description:`` Take off on a mission to Mars, or wherever your imagination takes you! These rocket wings use the latest in renewable energy, are kind to the environment, and approved by Growtech for commercial space flight!|0|3|350000|0||interface/large/gui_store_button_overlays1.rttex|0|0||-1|-1||1||||||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|2|\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|0|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|3|\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|1|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|");
					packet::storerequest(peer, "set_description_text|`2Weather Machines!``  Select the item you'd like more info on, or BACK to go back.|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||0|0|0|0||||-1|-1|||0||\nadd_tab_button|locks_menu|Locks And Stuff|interface/large/btn_shop.rttex||0|1|0|0||||-1|-1|||0||\nadd_tab_button|itempack_menu|Item Packs|interface/large/btn_shop.rttex||0|3|0|0||||-1|-1|||0||\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||0|4|0|0||||-1|-1|||0||\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky?  We offer alternatives within...|1|5|0|0||||-1|-1|||0||\nadd_tab_button|token_menu|Growtoken Items|interface/large/btn_shop.rttex||0|2|0|0||||-1|-1|||0||\nadd_button|weather_sunny|`oWeather Machine - Sunny``|interface/large/store_buttons/store_buttons5.rttex|`2You Get:`` 1 Weather Machine - Sunny.<CR><CR>`5Description:`` You probably don't need this one... but if you ever have a desire to turn a sunset or desert world back to normal, grab a Sunny Weather Machine to restore the default Growtopia sky! `5It's a perma-item, is never lost when destroyed.``|0|5|1000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|weather_night|`oWeather Machine - Night``|interface/large/store_buttons/store_buttons5.rttex|`2You Get:`` 1 Weather Machine - Night.<CR><CR>`5Description:`` You might not call it weather, but we do! This will turn the background of your world into a lovely night scene with stars and moon. `5It's a perma-item, is never lost when destroyed.``|0|6|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|weather_arid|`oWeather Machine - Arid``|interface/large/store_buttons/store_buttons5.rttex|`2You Get:`` 1 Weather Machine - Arid.<CR><CR>`5Description:`` Want your world to look like a cartoon desert? This will turn the background of your world into a desert scene with all the trimmings. `5It's a perma-item, is never lost when destroyed.``|0|7|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|weather_rainy|`oWeather Machine - Rainy City``|interface/large/store_buttons/store_buttons6.rttex|`2You Get:`` 1 Weather Machine - Rainy City.<CR><CR>`5Description:`` This will turn the background of your world into a dark, rainy city scene complete with sound effects. `5It's a perma-item, is never lost when destroyed.``|0|5|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|weather_warp|`oWeather Machine - Warp Speed``|interface/large/store_buttons/store_buttons11.rttex|`2You Get:`` 1 Weather Machine - Warp Speed.<CR><CR>`5Description:`` This Weather Machine will launch your world through space at relativistic speeds, which will cause you to age more slowly, as well as see stars flying by rapidly in the background. `5It's a perma-item, is never lost when destroyed.``|0|3|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|mars_blast|`oMars Blast``|interface/large/store_buttons/store_buttons6.rttex|`2You Get:`` 1 Mars Blast.<CR><CR>`5Description:`` Blast off to Mars!  This powerful rocket ship will launch you to a new world set up like the surface of Mars, with a special martian sky background, and unique terrain not found elsewhere in the solar system. Mars even has lower gravity than Growtopia normally does! Remember: When using this, you are creating a NEW world by typing in a new name. You can't convert an existing world to Mars, that would be dangerous.|0|7|15000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|thermo_blast|`oThermonuclear Blast``|interface/large/store_buttons/store_buttons8.rttex|`2You Get:`` 1 Thermonuclear Blast.<CR><CR>`5Description:`` This supervillainous device will blast you to a new world that has been scoured completely empty - it contains nothing but Bedrock and a White Door. Remember: When using this, you are creating a NEW world by typing in a new name. It would be irresponsible to let you blow up an entire existing world.|0|5|15000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|undersea_blast|`oUndersea Blast``|interface/large/store_buttons/store_buttons9.rttex|`2You Get:`` 1 Undersea Blast.<CR><CR>`5Description:`` Explore the ocean!  This advanced device will terraform a new world set up like the bottom of the ocean, with a special ocean background, and special blocks like Seaweed, Coral, Jellyfish, Sharks, and maybe a special surprise... Remember, by using this you are creating a NEW world by typing in a new name. You can't convert an existing world to an ocean, that would be dangerous.|0|7|15000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|cave_blast|`oCave Blast``|interface/large/store_buttons/store_buttons15.rttex|`2You Get:`` 1 Cave Blast.<CR><CR>`5Description:`` This explosive device will punch a hole in the ground, giving you a dark cavern to explore. There are even rumors of treasure and the entrance to ancient mines, hidden deep in the caves... but make sure you bring a World Lock. The blasted world is not locked when it's created, so lock it before somebody shows up! Remember: When using this, you are creating a NEW world by typing in a new name. You can't convert an existing world to a cave, that would be dangerous.|0|2|30000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|weather_stuff|`oWeather Machine - Stuff``|interface/large/store_buttons/store_buttons15.rttex|`2You Get:`` 1 Weather Machine - Stuff.<CR><CR>`5Description:`` This is the most fun weather imaginable - Choose any item from your inventory, adjust some settings, and watch it rain down from the sky! Or up, if you prefer reversing the gravity. `5It's a perma-item, is never lost when destroyed.``|0|6|50000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|weather_jungle|`oWeather Machine - Jungle``|interface/large/store_buttons/store_buttons16.rttex|`2You Get:`` 1 Weather Machine - Jungle.<CR><CR>`5Description:`` This weather machine will turn the background of your world into a steamy jungle. `5It's a perma-item, is never lost when destroyed.``|0|5|20000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|weather_backgd|`oWeather Machine - Background``|interface/large/store_buttons/store_buttons17.rttex|`2You Get:`` 1 Weather Machine - Background.<CR><CR>`5Description:`` This amazing device can scan any Background Block, and will make your entire world look like it's been filled with that block. Also handy for hiding music notes! `5It's a perma-item, is never lost when destroyed.``|0|1|150000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|starship_blast|`oImperial Starship Blast``|interface/large/store_buttons/store_buttons21.rttex|`2You Get:`` 1 Imperial Starship Blast.<CR><CR>`5Description:`` Command your very own Starship and explore the cosmos! This blast contains one of 3 possible Imperial ship types - which will you get? Note: Each Starship comes with a full tank of gas, an Imperial Helm - Mk. I, Imperial Reactor - Mk. I and an Imperial Viewscreen - Mk. I, so you'll be all set for your adventure among the stars! Note: A Starship also comes with an assortment of space-age blocks!|0|1|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|digital_rain_weather|`oWeather Machine - Digital Rain``|interface/large/store_buttons/store_buttons22.rttex|`2You Get:`` 1 Weather Machine - Digital Rain.<CR><CR>`5Description:`` Take the grow pill, and we'll show you how deep the rabbit hole goes! Splash the scrolling code of creation across the skies of your worlds. They say you learn to understand it after a while... Note: You can only have one of these per world. `5It's a perma-item, is never lost when destroyed.``|0|6|30000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|treasure_blast|`oTreasure Blast``|interface/large/store_buttons/store_buttons26.rttex|`2You Get:`` 1 Treasure Blast.<CR><CR>`5Description:`` Enter a world of snow-capped peaks and long-forgotten mysteries! Riddles and secrets - and a ton of treasure - await those who brave this blast's blocks! Remember, when you use this, it'll create a new world by typing in a new name! No sense in searching for clues to great treasures in well-trod worlds, is there?|0|6|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|surg_blast|`oSurgWorld Blast``|interface/large/store_buttons/store_buttons27.rttex|`2You Get:`` 1 SurgWorld Blast and 1 Caduceaxe.<CR><CR>`5Description:`` Your gateway to a world of medical wonders! SurgWorld is a place of care and healing, with all kinds of interesting blocks, top tips on how to treat people with surgery, and an increased chance of getting maladies while you work! Also comes with 1 Caduceaxe to extract Vaccine Drops from blocks. `6Warning:`` May break when extracting vaccine.|0|2|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|bountiful_blast|`oBountiful Blast``|interface/large/store_buttons/store_buttons27.rttex|`2You Get:`` 1 Bountiful Blast.<CR><CR>`5Description:`` Enter a world of fertile soil, cheerful sunshine and lush green hills, and bountiful new trees! This blast is your ticket to a different kind of farming!|0|3|5000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|infinity_weather_machine|`oInfinity Weather Machine``|interface/large/store_buttons/store_buttons32.rttex|`2You Get:`` 1 Infinity Weather Machine.<CR><CR>`5Description:`` Imagine being able to predict the weather?! Well, with the Infinity Weather Machine you can! Add multiple Weather Machines to this machine and have them play on a loop, like a weather mix tape, kind of!|0|3|50000|0|||-1|-1||-1|-1||1||||||0|");
				}
				string buygrtext = "action|buy\nitem|token";
				if (cch.find(buygrtext) == 0) {
					//packet::storerequest(peer, "set_description_text|Welcome to the `2Growtopia Store``! Select the item you'd like more info on.`o. `wWant to get `5Supporter`` status? Any Gem purchase (or`57,000`` Gems earned with free tapjoy`` offers) will make you one. You'll get new skin colors, the `5Recycle`` tool to convert unwanted items into Gems, and more bonuses!|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||0|0|0|0||||-1|-1|||0|\nadd_tab_button|locks_menu|Locks and Stuff|interface/large/btn_shop.rttex||0|1|0|0||||-1|-1|||0|\nadd_tab_button|itempack_menu|Item packs|interface/large/btn_shop.rttex||0|3|0|0||||-1|-1|||0|\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||0|4|0|0||||-1|-1|||0|\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky? We offer alternatives within...|0|5|0|0||||-1|-1|||0|\nadd_tab_button|token_menu|Growtoken items|interface/large/btn_shop.rttex||1|2|0|0||||-1|-1|||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|1|\nadd_button|itemomonth|`oItem Of The Month``|interface/large/store_buttons/store_buttons16.rttex|April 2021:`` `9Grow X Rocket Wings``!<CR><CR>`2You Get:`` 1 `9Grow X Rocket Wings``.<CR><CR>`5Description:`` Take off on a mission to Mars, or wherever your imagination takes you! These rocket wings use the latest in renewable energy, are kind to the environment, and approved by Growtech for commercial space flight!|0|3|350000|0||interface/large/gui_store_button_overlays1.rttex|0|0||-1|-1||1||||||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|2|\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|0|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|3|\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|1|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|");
					packet::storerequest(peer, "set_description_text|`2Spend your Growtokens!`` (You have `50``) You earn Growtokens from Crazy Jim and Sales-Man. Select the item you'd like more info on, or BACK to go back.|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||0|0|0|0||||-1|-1|||0||\nadd_tab_button|locks_menu|Locks And Stuff|interface/large/btn_shop.rttex||0|1|0|0||||-1|-1|||0||\nadd_tab_button|itempack_menu|Item Packs|interface/large/btn_shop.rttex||0|3|0|0||||-1|-1|||0||\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||0|4|0|0||||-1|-1|||0||\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky?  We offer alternatives within...|0|5|0|0||||-1|-1|||0||\nadd_tab_button|token_menu|Growtoken Items|interface/large/btn_shop.rttex||1|2|0|0||||-1|-1|||0||\nadd_button|challenge_timer|`oChallenge Timer``|interface/large/store_buttons/store_buttons15.rttex|`2You Get:`` 1 Challenge Timer.<CR><CR>`5Description:`` Get more people playing your parkours with this secure prize system. You'll need a `#Challenge Start Flag`` and `#Challenge End Flag`` as well (not included). Stock prizes into the Challenge Timer, set a time limit, and watch as players race from start to end. If they make it in time, they win a prize!|0|5|-5|0|||-1|-1||-1|-1||1||||||0||\nadd_button|xp_potion|`oExperience Potion``|interface/large/store_buttons/store_buttons9.rttex|`2You Get:`` 1 Experience Potion.<CR><CR>`5Description:`` This `#Untradeable`` delicious fizzy drink will make you smarter! 10,000 XP smarter instantly, to be exact.|0|2|-10|0|||-1|-1||-1|-1||1||||||0||\nadd_button|megaphone|`oMegaphone``|interface/large/store_buttons/store_buttons15.rttex|`2You Get:`` 1 Megaphone.<CR><CR>`5Description:`` You like broadcasting messages, but you're not so big on spending gems? Buy a Megaphone with Growtokens! Each Megaphone can be used once to send a super broadcast to all players in the game.|0|7|-10|0|||-1|-1||-1|-1||1||||||0||\nadd_button|growmoji_pack|`oGrowmoji Mystery Box``|interface/large/store_buttons/store_buttons19.rttex|`2You Get:`` 1 Growmoji.<CR><CR>`5Description:`` Express yourself! This mysterious box contains one of five fun growmojis you can use to spice up your chat! Which will you get?|0|1|-15|0|||-1|-1||-1|-1||1||||||0||\nadd_button|mini_mod|`oMini-Mod``|interface/large/store_buttons/store_buttons17.rttex|`2You Get:`` 1 Mini-Mod.<CR><CR>`5Description:`` Oh no, it's a Mini-Mod! Punch him to activate (you'll want to punch him!). When activated, he won't allow anyone to drop items in your world.|0|0|-20|0|||-1|-1||-1|-1||1||||||0||\nadd_button|derpy_star|`oDerpy Star Block``|interface/large/store_buttons/store_buttons10.rttex|`2You Get:`` 1 Derpy Star Block.<CR><CR>`5Description:`` DER IM A SUPERSTAR. This is a fairly ordinary block, except for the derpy star on it. Note: it is not permanent, and it doesn't drop seeds. So use it wisely!|0|3|-30|0|||-1|-1||-1|-1||1||||||0||\nadd_button|dirt_gun|`oBLYoshi's Free Dirt``|interface/large/store_buttons/store_buttons13.rttex|`2You Get:`` 1 BLYoshi's Free Dirt.<CR><CR>`5Description:`` \"Free\" might be stretching it, but hey, once you buy this deadly rifle, you can spew out all the dirt you want for free! Note: the dirt is launched at high velocity and explodes on impact. Sponsored by BLYoshi.|0|4|-40|0|||-1|-1||-1|-1||1||||||0||\nadd_button|nothingness|`oWeather Machine - Nothingness``|interface/large/store_buttons/store_buttons9.rttex|`2You Get:`` 1 Weather Machine - Nothingness.<CR><CR>`5Description:`` Tired of all that fancy weather?  This machine will turn your world completely black. Yup, that's it. Not a single pixel in the background except pure blackness.|0|3|-50|0|||-1|-1||-1|-1||1||||||0||\nadd_button|spike_juice|`oSpike Juice``|interface/large/store_buttons/store_buttons10.rttex|`2You Get:`` 1 Spike Juice.<CR><CR>`5Description:`` It's fresh squeezed, with little bits of spikes still in it! Drinking this `#Untradeable`` one-use potion will make you immune to Death Spikes and Lava for 5 seconds.|0|5|-60|0|||-1|-1||-1|-1||1||||||0||\nadd_button|doodad|`oDoodad``|interface/large/store_buttons/store_buttons9.rttex|`2You Get:`` 1 Doodad.<CR><CR>`5Description:`` I have no idea what this thing does. It's something electronic? Maybe?|0|5|-75|0|||-1|-1||-1|-1||1||||||0||\nadd_button|crystal_cape|`oCrystal Cape``|interface/large/store_buttons/store_buttons11.rttex|`2You Get:`` 1 Crystal Cape.<CR><CR>`5Description:`` This cape is woven of pure crystal, which makes it pretty uncomfortable. But it also makes it magical! It lets you double-jump off of an imaginary Crystal Block in mid-air. Sponsored by Edvoid20, HemeTems, and Aboge.|0|5|-90|0|||-1|-1||-1|-1||1||||||0||\nadd_button|focused_eyes|`oFocused Eyes``|interface/large/store_buttons/store_buttons9.rttex|`2You Get:`` 1 Focused Eyes.<CR><CR>`5Description:`` This `#Untradeable`` item lets you shoot electricity from your eyes! Wear them with pride, and creepiness.|0|4|-100|0|||-1|-1||-1|-1||1||||||0||\nadd_button|grip_tape|`oGrip Tape``|interface/large/store_buttons/store_buttons14.rttex|`2You Get:`` 1 Grip Tape.<CR><CR>`5Description:`` This is handy for wrapping around the handle of a weapon or tool. It can improve your grip, as well as protect you from cold metal handles. If you aren't planning to craft a weapon that requires Grip Tape, this does you no good at all!|0|5|-100|0|||-1|-1||-1|-1||1||||||0||\nadd_button|cat_eyes|`oCat Eyes``|interface/large/store_buttons/store_buttons23.rttex|`2You Get:`` 1 Cat Eyes.<CR><CR>`5Description:`` Wow, pawesome! These new eyes are the cat's meow, and the purrfect addition to any style.|0|5|-100|0|||-1|-1||-1|-1||1||||||0||\nadd_button|night_vision|`oNight Vision Goggles``|interface/large/store_buttons/store_buttons15.rttex|`2You Get:`` 1 Night Vision Goggles.<CR><CR>`5Description:`` Scared of the dark? We have a solution. You can wear these goggles just to look cool, but if you also happen to have a D Battery (`4batteries not included``) on you, you will be able to see through darkness like it's not even there! Each D Battery can power your goggles for 1 minute. `2If you are in a world you own, the goggles will not require batteries!`` Note: you can't turn the goggles off without removing them, so you'll be wasting your battery if you wear them in daylight while carrying D Batteries.|0|3|-110|0|||-1|-1||-1|-1||1||||||0||\nadd_button|muddy_pants|`oMuddy Pants``|interface/large/store_buttons/store_buttons12.rttex|`2You Get:`` 1 Muddy Pants.<CR><CR>`5Description:`` Well, this is just a pair of muddy pants. But it does come with a super secret bonus surprise that is sure to blow your mind!|0|7|-125|0|||-1|-1||-1|-1||1||||||0||\nadd_button|piranha|`oCuddly Piranha``|interface/large/store_buttons/store_buttons10.rttex|`2You Get:`` 1 Cuddly Piranha.<CR><CR>`5Description:`` This friendly pet piranha won't stay in its bowl!  It just wants to snuggle with your face!|0|0|-150|0|||-1|-1||-1|-1||1||||||0||\nadd_button|puddy_leash|`oPuddy Leash``|interface/large/store_buttons/store_buttons11.rttex|`2You Get:`` 1 Puddy Leash.<CR><CR>`5Description:`` Puddy is a friendly little kitten who will follow you around forever.|0|7|-180|0|||-1|-1||-1|-1||1||||||0||\nadd_button|golden_axe|`oGolden Pickaxe``|interface/large/store_buttons/store_buttons9.rttex|`2You Get:`` 1 Golden Pickaxe.<CR><CR>`5Description:`` Get your own sparkly pickaxe! This `#Untradeable`` item is a status symbol! Oh sure, it isn't any more effective than a normal pickaxe, but it sparkles!|0|1|-200|0|||-1|-1||-1|-1||1||||||0||\nadd_button|puppy_leash|`oPuppy Leash``|interface/large/store_buttons/store_buttons11.rttex|`2You Get:`` 1 Puppy Leash.<CR><CR>`5Description:`` Get your own pet puppy! This little dog will follow you around forever, never wavering in her loyalty, thus making her `#Untradeable``.|0|4|-200|0|||-1|-1||-1|-1||1||||||0||\nadd_button|diggers_spade|`oDigger's Spade``|interface/large/store_buttons/store_buttons13.rttex|`2You Get:`` 1 Digger's Spade.<CR><CR>`5Description:`` This may appear to be a humble shovel, but in fact it is enchanted with the greatest magic in Growtopia. It can smash Dirt or Cave Background in a single hit! Unfortunately, it's worthless at digging through anything else. Note: The spade is `#UNTRADEABLE``.|0|7|-200|0|||-1|-1||-1|-1||1||||||0||\nadd_button|meow_ears|`oMeow Ears``|interface/large/store_buttons/store_buttons22.rttex|`2You Get:`` 1 Meow Ears.<CR><CR>`5Description:`` Meow's super special ears that everyone can now get! Note: These ears are `#UNTRADEABLE``.|0|0|-200|0|||-1|-1||-1|-1||1||||||0||\nadd_button|frosty_hair|`oFrosty Hair``|interface/large/store_buttons/store_buttons23.rttex|`2You Get:`` 1 Frosty Hair.<CR><CR>`5Description:`` Coldplay is cold, but you can be freezing! Note: The frosty hair is `#UNTRADEABLE``.|0|0|-200|0|||-1|-1||-1|-1||1||||||0||\nadd_button|zerkon_helmet|`oEvil Space Helmet``|interface/large/store_buttons/store_buttons21.rttex|`2You Get:`` 1 Evil Space Helmet.<CR><CR>`5Description:`` Zerkon commands a starship too small to actually board - pah, time to rule the galaxy properly! Note: The evil space helmet is `#UNTRADEABLE``.|0|6|-200|0|||-1|-1||-1|-1||1||||||0||\nadd_button|seils_magic_orb|`oSeil's Magic Orbs``|interface/large/store_buttons/store_buttons21.rttex|`2You Get:`` 1 Seil's Magic Orbs.<CR><CR>`5Description:`` Seil is some kind of evil wizard, now you can be too! Note: These magic orbs are `#UNTRADEABLE``.|0|7|-200|0|||-1|-1||-1|-1||1||||||0||\nadd_button|atomic_shadow_scythe|`oAtomic Shadow Scythe``|interface/large/store_buttons/store_buttons21.rttex|`2You Get:`` 1 Atomic Shadow Scythe.<CR><CR>`5Description:`` AtomicShadow might actually be evil, now you can try it out! Note: The shadow scythe is `#UNTRADEABLE``.|0|5|-200|0|||-1|-1||-1|-1||1||||||0||\nadd_button|poseidon_diggers_trident|`oPoseidon's Digger's Trident``|interface/large/store_buttons/store_buttons25.rttex|`2You Get:`` 1 Poseidon's Digger's Trident.<CR><CR>`5Description:`` A gift from the gods. This may appear to be a humble trident, but in fact it has the power of Poseidon himself. It can smash `8Deep Sand`` or `8Ocean Rock`` in a single hit. Unfortunately, you don't get to wield the full might of Poseidon... the trident is worthless at smashing anything else. Note: The trident is `#UNTRADEABLE``.|0|6|-200|0|||-1|-1||-1|-1||1||||||0||\nadd_button|grow_boy|`oGrowBoy``|interface/large/store_buttons/store_buttons32.rttex|`2You Get:`` 1 Growboy.<CR><CR>`5Description:`` Bask in the nostalgic green screened goodness of the Growboy! A portable gaming device that packs a punch. Now you're playing with GrowPower! Note: The Growboy is `#UNTRADEABLE``.|0|2|-100|0|||-1|-1||-1|-1||1||||||0||\nadd_button|tsed|`oTactical Stealth Espionage Device``|interface/large/store_buttons/store_buttons32.rttex|`2You Get:`` 1 Tactical Stealth Espionage Device.<CR><CR>`5Description:`` This is Growtech's latest innovation on tactical espionage! Using the latest in scientific breakthroughs this device allows you to seamlessly disguise yourself as... a cardboard box! Note: The Tactical Stealth Espionage Device is `#UNTRADEABLE``.|0|1|-150|0|||-1|-1||-1|-1||1||||||0||\nadd_button|really_dangerous_pet_llama|`oReally Dangerous Pet Llama``|interface/large/store_buttons/store_buttons32.rttex|`2You Get:`` 1 Really Dangerous Pet Llama.<CR><CR>`5Description:`` This Llama is ready for anything Growtopia throws at it! Armed with a silo of Growtech Missiles, experimental Growtech hardened steel armor and a rather snazzy helmet (Llama's own) this Llama is Dangerous with a capital D! Note: This Really Dangerous Pet Llama is `#UNTRADEABLE``.|0|0|-200|0|||-1|-1||-1|-1||1||||||0|");
				}
				string buygang = "action|buy\nitem|gang_pack";
				if (cch.find(buygang) == 0) {
					int price = 5000;
					if (((PlayerInfo*)(peer->data))->gems >= price) {
						((PlayerInfo*)(peer->data))->gems = ((PlayerInfo*)(peer->data))->gems - price;
						packet::storepurchaseresult(peer, "`wYou've purchased `oGangland Pack`w for `o5,000`w Gems.<CR>`wYou have`o " + std::to_string(((PlayerInfo*)(peer->data))->gems) + "`w Gems left.<CR><CR> `5Recieved: `w1 Fedora, 1 Dames Fedora, 1 Pinstripe Suit with Pants, 1 Flapper Headband with Dress, 1 Cigar, 1 Tommy Gun, 1 Victola and 10 Art Deco Blocks.");
						packet::setbux(peer, ((PlayerInfo*)(peer->data))->gems);
						string text = "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0\n";
						BYTE* data = new BYTE[5 + text.length()];
						BYTE zero = 0;
						int type = 3;
						memcpy(data, &type, 4);
						memcpy(data + 4, text.c_str(), text.length());
						memcpy(data + 4 + text.length(), &zero, 1);
						ENetPacket * packet2 = enet_packet_create(data,
							5 + text.length(),
							ENET_PACKET_FLAG_RELIABLE);

						enet_peer_send(peer, 0, packet2);
						//packet::consolemessage(peer, "`oRecieved: `w1 Grow X Rocket Wings");
						PlayerInventory inventory = ((PlayerInfo *)(peer->data))->inventory;
						InventoryItem fist, wrench, item1, item2, item3, item4, item5, item6, item7, item8, item9, item10;
						int itemID = 462;
						int itemPunch = 18;
						int itemWrench = 32;
						if (itemID != 112 && itemID != 18 && itemID != 32) {
							item1.itemID = itemID;//fedora
							item2.itemID = 464;//Dame's fedora
							item3.itemID = 466;//Pinstripe suit
							item4.itemID = 468;//pinstripe pants
							item5.itemID = 470;//cigar
							item6.itemID = 472;//tommygun
							item7.itemID = 474;//flapper headband
							item8.itemID = 476;//flapper dress
							item9.itemID = 478;//art deco
							item10.itemID = 480;//victrola

							item1.itemCount = 1;
							item2.itemCount = 1;
							item3.itemCount = 1;
							item4.itemCount = 1;
							item5.itemCount = 1;
							item6.itemCount = 1;
							item7.itemCount = 1;
							item8.itemCount = 1;
							item10.itemCount = 1;
							item9.itemCount = 10;

							fist.itemID = itemPunch;
							wrench.itemID = itemWrench;
							fist.itemCount = 1;
							wrench.itemCount = 1;

							inventory.items.push_back(item1);
							inventory.items.push_back(item2);
							inventory.items.push_back(item3);
							inventory.items.push_back(item4);
							inventory.items.push_back(item5);
							inventory.items.push_back(item6);
							inventory.items.push_back(item7);
							inventory.items.push_back(item8);
							inventory.items.push_back(item9);
							inventory.items.push_back(item10);
							inventory.items.push_back(fist);
							inventory.items.push_back(wrench);
							sendInventory(peer, inventory);
						}
						sendSaveSet(peer);
					}
					else {
						int price = 5000;
						int error = price - ((PlayerInfo*)(peer->data))->gems;
						packet::storepurchaseresult(peer, "`wYou can't afford the `oGangland Style`w Pack! You are`$ " + std::to_string(error) + "`w Gems short.");
						string text = "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0\n";
						BYTE* data = new BYTE[5 + text.length()];
						BYTE zero = 0;
						int type = 3;
						memcpy(data, &type, 4);
						memcpy(data + 4, text.c_str(), text.length());
						memcpy(data + 4 + text.length(), &zero, 1);
						ENetPacket * packet2 = enet_packet_create(data,
							5 + text.length(),
							ENET_PACKET_FLAG_RELIABLE);

						enet_peer_send(peer, 0, packet2);
					}
				}
				string buyitemom = "action|buy\nitem|itemomonth";
				if (cch.find(buyitemom) == 0) {
					//packet::storepurchaseresult(peer, "Item of the Month cannot be purchased yet!");
				//	ENetPeer * peer;//helpme
					int price = 350000;
					//hasrocketwing
					//if (((PlayerInfo*)(peer->data))->hasLegenName == false) {
					if (((PlayerInfo*)(peer->data))->gems >= price) {
						((PlayerInfo*)(peer->data))->gems = ((PlayerInfo*)(peer->data))->gems - price;
						((PlayerInfo*)(peer->data))->hasRocketWing = true;
						packet::storepurchaseresult(peer, "`wYou've purchased `oGrow X Rocket Wings`w for `o350,000`w Gems.<CR>`wYou have`o " + std::to_string(((PlayerInfo*)(peer->data))->gems) + "`w Gems left.<CR><CR> `5Recieved: `w1 Grow X Rocket Wings");
						packet::setbux(peer, ((PlayerInfo*)(peer->data))->gems);
						string text = "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0\n";
						BYTE* data = new BYTE[5 + text.length()];
						BYTE zero = 0;
						int type = 3;
						memcpy(data, &type, 4);
						memcpy(data + 4, text.c_str(), text.length());
						memcpy(data + 4 + text.length(), &zero, 1);
						ENetPacket * packet2 = enet_packet_create(data,
							5 + text.length(),
							ENET_PACKET_FLAG_RELIABLE);

						enet_peer_send(peer, 0, packet2);
						packet::consolemessage(peer, "`oRecieved: `w1 Grow X Rocket Wings");
						PlayerInventory inventory = ((PlayerInfo *)(peer->data))->inventory;
						InventoryItem item, item1, item2;
						int itemID = 10718;
						int itemPunch = 18;
						int itemWrench = 32;
						if (itemID != 112 && itemID != 18 && itemID != 32) {
							item.itemID = itemID;
							item1.itemID = itemPunch;
							item2.itemID = itemWrench;
							item.itemCount = + 1;
							item1.itemCount = 1;
							item2.itemCount = 1;

							inventory.items.push_back(item);
							inventory.items.push_back(item1);
							inventory.items.push_back(item2);
							sendInventory(peer, inventory);
						}
						sendSaveSet(peer);
					}
					else {
						int price = 350000;
						int error = price - ((PlayerInfo*)(peer->data))->gems;
						packet::storepurchaseresult(peer, "`wYou can't afford `oGrow X Rocket Wings`w! You are`$ " + std::to_string(error) + "`w Gems short.");
						string text = "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0\n";
						BYTE* data = new BYTE[5 + text.length()];
						BYTE zero = 0;
						int type = 3;
						memcpy(data, &type, 4);
						memcpy(data + 4, text.c_str(), text.length());
						memcpy(data + 4 + text.length(), &zero, 1);
						ENetPacket * packet2 = enet_packet_create(data,
							5 + text.length(),
							ENET_PACKET_FLAG_RELIABLE);

						enet_peer_send(peer, 0, packet2);
					}

				}
				if (cch.find("action|respawn") == 0)
				{
					int x = 3040;
					int y = 736;

					if (!world) continue;

					for (int i = 0; i < world->width * world->height; i++)
					{
						if (world->items[i].foreground == 6) {
							x = (i % world->width) * 32;
							y = (i / world->width) * 32;
						}
					}
					{
						PlayerMoving data;
						data.packetType = 0x0;
						data.characterState = 0x924; // animation
						data.x = x;
						data.y = y;
						data.punchX = -1;
						data.punchY = -1;
						data.XSpeed = 0;
						data.YSpeed = 0;
						data.netID = ((PlayerInfo*)(peer->data))->netID;
						data.plantingTree = 0x0;
						SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
					}

					{
						int x = 3040;
						int y = 736;

						for (int i = 0; i < world->width * world->height; i++)
						{
							if (world->items[i].foreground == 6) {
								x = (i % world->width) * 32;
								y = (i / world->width) * 32;
							}
						}

						gamepacket_t p(0, ((PlayerInfo*)(peer->data))->netID);
						p.Insert("OnSetPos");
						p.Insert(x, y);
						p.CreatePacket(peer);
					}
					{
						int x = 3040;
						int y = 736;

						for (int i = 0; i < world->width * world->height; i++)
						{
							if (world->items[i].foreground == 6) {
								x = (i % world->width) * 32;
								y = (i / world->width) * 32;
							}
						}

						gamepacket_t p(0, ((PlayerInfo*)(peer->data))->netID);
						p.Insert("OnSetFreezeState");
						p.Insert(0);
						p.CreatePacket(peer);
					}
#ifdef TOTAL_LOG
					cout << "Respawning... " << endl;
#endif
				}
				if (cch.find("action|growid") == 0)
				{
						//GamePacket p = packetEnd(appendString(appendString(createPacket(), "OnDialogRequest"), "set_default_color|`o\n\nadd_label_with_icon|big|`w" + itemDefs.at(id).name + "``|left|" + std::to_string(id) + "|\n\nadd_spacer|small|\nadd_textbox|" + itemDefs.at(id).description + "|left|\nadd_spacer|small|\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|"));
					gamepacket_t p(2000);
					p.Insert("OnDialogRequest");
					p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`wGet a GrowID``|left|206|\n\nadd_spacer|small|\nadd_textbox|A `wGrowID `wmeans `oyou can use a name and password to logon from any device.|\nadd_spacer|small|\nadd_textbox|This `wname `owill be reserved for you and `wshown to other players`o, so choose carefully! `wNote`o please do not use `w'of Legend'`o as you won't be able to login.|\nadd_text_input|username|GrowID||30|\nadd_text_input|password|Password||100|\nadd_text_input|passwordverify|Password Verify||100|\nadd_textbox|Your `wemail address `owill only be used for account verification purposes and won't be spammed or shared. If you use a fake email, you'll never be able to recover or change your password.|\nadd_text_input|email|Email||100|\nend_dialog|register|Cancel|Get My GrowID!|\n");
					p.CreatePacket(peer);
					//packet::dialog(peer, "set_default_color|`o\n\nadd_label_with_icon|big|`wCreating an Account``|left|1280|\n\nadd_spacer|small|\nadd_textbox|Welcome to `wGTSF Server`o by creating your Account you will have full access on some of our features!|\nadd_spacer|small|\nadd_textbox|Please kindly make your Username! Reminder : Badwords/Sexual Content words on your name is Illegal!|\nadd_text_input|username|Username :||30|\nadd_text_input|password|Password :||100|\nadd_text_input|passwordverify|Re-Enter Password :||100|\nend_dialog|register|Cancel|Create my Account!|\n");
						packet::consolemessage(peer, "`otest");
				}
				if (cch.find("action|store") == 0)
				{
					//shopers
					//packet::storerequest(peer, "set_description_text|Welcome to the `2Growtopia Store``! Select the item you'd like more info on.`o. `wWant to get `5Supporter`` status? Any Gem purchase (or`57,000`` Gems earned with free tapjoy`` offers) will make you one. You'll get new skin colors, the `5Recycle`` tool to convert unwanted items into Gems, and more bonuses!|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||1|0|0|0||||-1|-1|||0|\nadd_tab_button|locks_menu|Locks and Stuff|interface/large/btn_shop.rttex||0|1|0|0||||-1|-1|||0|\nadd_tab_button|itempack_menu|Item packs|interface/large/btn_shop.rttex||0|3|0|0||||-1|-1|||0|\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||0|4|0|0||||-1|-1|||0|\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky? We offer alternatives within...|0|5|0|0||||-1|-1|||0|\nadd_tab_button|token_menu|Growtoken items|interface/large/btn_shop.rttex||0|2|0|0||||-1|-1|||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|1|\nadd_button|itemomonth|`oItem Of The Month``|interface/large/store_buttons/store_buttons16.rttex|April 2021:`` `9Grow X Rocket Wings``!<CR><CR>`2You Get:`` 1 `9Grow X Rocket Wings``.<CR><CR>`5Description:`` Take off on a mission to Mars, or wherever your imagination takes you! These rocket wings use the latest in renewable energy, are kind to the environment, and approved by Growtech for commercial space flight!|0|3|350000|0||interface/large/gui_store_button_overlays1.rttex|0|0||-1|-1||1||||||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|2|\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|0|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|3|\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|1|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|");
					//packet::storerequest(peer, "set_description_text|Welcome to the `2Growtopia Store``! Select the item you'd like more info on.`o. `wWant to get `5Supporter`` status? Any Gem purchase (or`57,000`` Gems earned with free tapjoy`` offers) will make you one. You'll get new skin colors, the `5Recycle`` tool to convert unwanted items into Gems, and more bonuses!|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||1|0|0|0||||-1|-1|||0|\nadd_tab_button|locks_menu|Locks and Stuff|interface/large/btn_shop.rttex||0|1|0|0||||-1|-1|||0|\nadd_tab_button|itempack_menu|Item packs|interface/large/btn_shop.rttex||0|3|0|0||||-1|-1|||0|\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||0|4|0|0||||-1|-1|||0|\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky? We offer alternatives within...|0|5|0|0||||-1|-1|||0|\nadd_tab_button|token_menu|Growtoken items|interface/large/btn_shop.rttex||0|2|0|0||||-1|-1|||0|\nadd_banner|interface/large/gui_shop_winter_seals.rttex|bannerlayout|OPENDIALOG|grow4goodcommunity|\nadd_button|rt_grope_eventpass_bundle01|Grow4Good Pass|interface/large/store_buttons/store_buttons35.rttex|https://growtopiagame.com|1|1|0||||-1|-1|`2You Get:`` Grow4Good Pass, and 350k gems. 1 Imperial Starship Blast, 3 Startopia Supply Crates, 3 Surgery Supply Crates, 3 Fishing Supply Crates, 3 Cooking Supply Crates, 3 Superhero Supply Crates instantly.<CR><CR>`5Description:`` Grow4Good Pass gives access to earn exclusive rewards including the One Winged Angel when you progress in the Grow4Good Personal Contributions as well as everything you need to enjoy all the activites during the event. <CR><CR>`210% of the revenue from all `5Grow4Good Passess`` sold throught the event will be donated to Operation Smile!``|1||||||0|\nadd_button|grow4good_personal|`oGrow4Good Personal Points|interface/large/store_buttons/store_buttons35.rttex|OPENDIALOG|&grow4goodpersonal|0|2|0|0|||-1|-1||-1|-1||1||||||0|\nadd_button|rt_grope_cosmetic_bundle01|`oOne Winged Angel``|interface/large/store_buttons/store_buttons35.rttex|https://growtopiagame.com|1|0|0||||-1|-1||-1|-1|`2You Get:`` 1 One Winged Angel Item.<CR><CR>`5Description:`` This single wing is a symbol for all those who strive to overcome adversity, no matter what the odds. To avoid refund fraud, this item has been made untradeable until August 25th, 2021.|1||||||0||\nadd_button|wavey_arm_guy|`oWaving Inflateable Arm Guy|interface/large/shop_buttons/shop_buttons35.rttex|`2You Get:`` 1 Waving Inflateable Arm Guy.<CR><CR>`5Description: ``Show your support for the Grow4Good Drive when you buy this incredible Waving Inflateable Arm Guy! Advertise how cool your world is with this cute guy waiting to greet everyone that enters.|1|2|50000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|itemomonth|`oItem Of The Month``|interface/large/store_buttons/store_buttons16.rttex|April 2021:`` `9Grow X Rocket Wings``!<CR><CR>`2You Get:`` 1 `9Grow X Rocket Wings``.<CR><CR>`5Description:`` Take off on a mission to Mars, or wherever your imagination takes you! These rocket wings use the latest in renewable energy, are kind to the environment, and approved by Growtech for commercial space flight!|0|3|350000|0||interface/large/gui_store_button_overlays1.rttex|0|0||-1|-1||1||||||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|2|\nadd_button|gems_glory|Road To Glory|interface/large/store_buttons/store_buttons30.rttex|https://growtopiagame.com|0|0|0|||interface/large/gui_store_buttons_overlays1.rttex|0|0|/interface/large/gui_shop_buybanner.rttex|1|0|`2You Get:`` Road To Glory and 100k Gems Instantly.<CR>`5Description:`` Earn Gem rewards when you level up. Every 10 levels you will get additional Gem rewards up until level 50! Claim all rewards instantly if you are over Level 50!! 1.6m Gems in total! |1||||||0||\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|1|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|");
				// --- new store --
					//packet::storerequest(peer, "set_description_text|Welcome to the `2Growtopia Store``! Select the item you'd like more info on.`o `wWant to get `5Supporter`` status? Any Gem purchase (or `57,000`` Gems earned with free `5Tapjoy`` offers) will make you one. You'll get new skin colors, the `5Recycle`` tool to convert unwanted items into Gems, and more bonuses!|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||1|0|0|0||||-1|-1|||0||\nadd_tab_button|locks_menu|Locks And Stuff|interface/large/btn_shop.rttex||0|1|0|0||||-1|-1|||0||\nadd_tab_button|itempack_menu|Item Packs|interface/large/btn_shop.rttex||0|3|0|0||||-1|-1|||0||\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||0|4|0|0||||-1|-1|||0||\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky?  We offer alternatives within...|0|5|0|0||||-1|-1|||0||\nadd_tab_button|token_menu|Growtoken Items|interface/large/btn_shop.rttex||0|2|0|0||||-1|-1|||0||\nadd_image_button|banner|interface/large/gui_shop_winter_seals.rttex|bannerlayout|OPENDIALOG|grow4goodcomunity||\nadd_button|rt_grope_eventpass_bundle01|Grow4Good Pass|interface/large/store_buttons/store_buttons35.rttex|https://growtopiagame.com|1|1|0||||-1|-1||-1|-1|`2You Get:`` Grow4Good Pass, and 350k Gems. 1 Imperial Starship Blast, 3 Startopia Supply Crates, 3 Surgery Supply Crates, 3 Fishing Supply Crates, 3 Cooking Supply Crates, 3 Superhero Supply Crates Instantly.<CR><CR>`5Description:`` Grow4Good Pass gives access to earn exclusive rewards incuding the One Winged Angel when you progress in the Grow4Good Personal Contributions as well as everything you need to enjoy all the activities during the event.<CR><CR>`210% of the revenue from all `5Grow4Good Passes`` sold throughout the event will be donated to Operation Smile!``|1||||||0||\nadd_button|grow4good_personal|`oGrow4Good Personal Points|interface/large/store_buttons/store_buttons35.rttex|OPENDIALOG&grow4goodpersonal|0|2|0|0|||-1|-1||-1|-1||1||||||0||\nadd_button|rt_grope_cosmetic_bundle01|`oOne Winged Angel``|interface/large/store_buttons/store_buttons35.rttex|https://growtopiagame.com|1|0|0||||-1|-1||-1|-1|`2You Get:`` 1 One Winged Angel Item.<CR><CR>`5Description:`` This single wing is a symbol for all those who strive to overcome adversity, no matter what the odds. To avoid refund fraud, this item has been made untradeable until August 25th, 2021.|1||||||0||\nadd_button|wavey_arm_guy|`oWaving Inflatable Arm Guy|interface/large/store_buttons/store_buttons35.rttex|`2You Get:`` 1 Waving Inflatable Arm Guy.<CR><CR>`5Description:`` Show your support for the Grow4Good Drive when you buy this incredible Waving Inflatable Arm Guy! Advertise just how cool your world is with this cute guy waiting to greet everyone that enters.|1|2|50000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|gross_mixers|`oBag of Mutant Mixers``|interface/large/store_buttons/store_buttons27.rttex|`2You Get:`` 10 Rotting Sea Goo, 10 Space Poop, 10 Burnt Slime, 10 Putrid Gas and 10 Strawberry Slime Blocks and 1 Random Mutant Mamma's Cooking Recipe.<CR><CR>`5Description:`` A bag of `2Mutant Mixers`` to make deliciously mutated ingredients and one of `5Mutant Mumma's`` very own mutated recipes to cook with them! You also get some slippery `2Strawberry Slime Blocks``.|0|0|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|itemomonth|`oItem Of The Month``|interface/large/store_buttons/store_buttons16.rttex|April 2021:`` `9Grow X Rocket Wings``!<CR><CR>`2You Get:`` 1 `9Grow X Rocket Wings``.<CR><CR>`5Description:`` Take off on a mission to Mars, or wherever your imagination takes you! These rocket wings use the latest in renewable energy, are kind to the environment, and approved by Growtech for commercial space flight!|0|3|350000|0||interface/large/gui_store_button_overlays1.rttex|0|0||-1|-1||1||||||0||\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|2||\nadd_button|gems_glory|Road To Glory|interface/large/store_buttons/store_buttons30.rttex|https://growtopiagame.com|0|0|0|||interface/large/gui_store_button_overlays1.rttex|0|0|/interface/large/gui_shop_buybanner.rttex|1|0|`2You Get:`` Road To Glory and 100k Gems Instantly.<CR>`5Description:`` Earn Gem rewards when you level up. Every 10 levels you will get additional Gem rewards up to Level 50! Claim all rewards instantly if you are over level 50!! 1.6M Gems in total!! |1||||||0||\nadd_button|gems_bundle05|Gem Bounty|interface/large/store_buttons/store_buttons34.rttex|https://growtopiagame.com|0|6|0||||-1|-1||-1|-1|`2You Get:`` 900,000 Gems, 6 Growtokens and 3 Megaphones.<CR><CR>`5Description:`` Get a plethora of gems to add to your wealth.|1||||||0||\nadd_button|gems_rain|It's Rainin' Gems|interface/large/store_buttons/store_buttons.rttex|https://growtopiagame.com|1|5|0||||-1|-1||-1|-1|`2You Get:`` 290,000 Gems, 2 Growtokens and 1 Megaphone.<CR><CR>`5Description:`` All the gems you could ever want and more plus 2 Growtokens and a Megaphone to tell the whole world about it.|1||||||0||\nadd_button|gems_fountain|Gem Fountain|interface/large/store_buttons/store_buttons2.rttex|https://growtopiagame.com|0|2|0||||-1|-1||-1|-1|`2You Get:`` 130,000 Gems and 1 Growtoken.<CR><CR>`5Description:`` Get a pile of gems to shop to your hearts desire and 1 Growtoken.|1||||||0||\nadd_button|gems_chest|Chest o' Gems|interface/large/store_buttons/store_buttons.rttex|https://growtopiagame.com|0|5|0||||-1|-1||-1|-1|`2You Get:`` 45,000 Gems.<CR><CR>`5Description:`` Get a chest containing gems.|1||||||0||\nadd_button|gems_bag|Bag o' Gems|interface/large/store_buttons/store_buttons.rttex|https://growtopiagame.com|1|0|0||||-1|-1||-1|-1|`2You Get:`` 20,000 Gems.<CR><CR>`5Description:`` Get a small bag of gems.|1||||||0||\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|3||\nadd_button|365d|`o1-Year Subscription Token``|interface/large/store_buttons/store_buttons28.rttex|https://growtopiagame.com|0|2|0||||-1|-1||-1|-1|`2You Get:`` 1x 1-Year Subscription Token and 25 Growtokens.<CR><CR>`5Description:`` One full year of special treatment AND 25 Growtokens upfront! You'll get 70 season tokens (as long as there's a seasonal clash running), and 4000 gems every day and a chance of doubling any XP earned, growtime reduction on all seeds planted and Exclusive Skins!|1||||||0||\nadd_button|30d|`o30-Day Subscription Token``|interface/large/store_buttons/store_buttons28.rttex|https://growtopiagame.com|0|3|0||||-1|-1||-1|-1|`2You Get:`` 1x 30-Day Free Subscription Token and 2 Growtokens.<CR><CR>`5Description:`` 30 full days of special treatment AND 2 Growtokens upfront! You'll get 70 season tokens (as long as there's a seasonal clash running), and 4000 gems every day and a chance of doubling any XP earned, growtime reduction on all seeds planted and Exclusive Skins!|1||||||0|");
					packet::storerequest(peer, "set_description_text|Welcome to the `2Growtopia Store``! Select the item you'd like more info on.`o `wWant to get `5Supporter`` status? Any Gem purchase (or `57,000`` Gems earned with free `5Tapjoy`` offers) will make you one. You'll get new skin colors, the `5Recycle`` tool to convert unwanted items into Gems, and more bonuses!||\nadd_button|world_lock|`oWorld Lock``|interface/large/store_buttons/store_buttons.rttex|`2You Get:`` 1 World Lock.<CR><CR>`5Description:`` Become the undisputed ruler of your domain with one of these babies.  It works like a normal lock except it locks the `$entire world``!  Won't work on worlds that other people already have locks on. You can even add additional normal locks to give access to certain areas to friends. `5It's a perma-item, is never lost when destroyed.``  `wRecycles for 200 Gems.``|0|2||0|||-1|-1||-1|-1||1||||||0||\nadd_button|dungeon_pack|`oDungeon Pack``|interface/large/store_buttons/store_buttons4.rttex|`2You Get:`` 20 Grimstone, 20 Blackrock Wall, 20 Iron Bars, 3 Jail Doors, 3 Skeletons, 1 Headsman's Axe, 1 Worthless Rags. 5 Torches and a `#Rare Iron Mask!``.<CR><CR>`5Description:`` Lock up your enemies in a dank dungeon! Of course they can still leave whenever they want. But they won't want to, because it looks so cool! Iron Mask muffles your speech!|0|7|10000|0|||-1|-1||-1|-1||1||||||0||\nadd_button|oldlockssuff|`oItem Packs``|interface/large/store_buttons/store_buttons3.rttex|`2You Get:`` 20 Sheet Music: Blank, 20 Sheet Music: Piano Note, 20 Sheet Music: Bass Note, 20 Sheet Music Drums, 5 Sheet Music: Sharp Piano, 5 Sheet Music: Flat Piano, 5 Sheet Music: Flat Bass and 5 Sheet Music: Sharp Bass .<CR><CR>`5Description:`` With these handy blocks, you'll be able to compose your own music, using your World-Locked world as a sheet of music. Requires a World Lock (sold separately!).|0|3||0|||-1|-1||-1|-1||1||||||0||\nadd_button|oldlocksstuff|`oComposer's Pack``|interface/large/store_buttons/store_buttons3.rttex|`2You Get:`` 20 Sheet Music: Blank, 20 Sheet Music: Piano Note, 20 Sheet Music: Bass Note, 20 Sheet Music Drums, 5 Sheet Music: Sharp Piano, 5 Sheet Music: Flat Piano, 5 Sheet Music: Flat Bass and 5 Sheet Music: Sharp Bass .<CR><CR>`5Description:`` With these handy blocks, you'll be able to compose your own music, using your World-Locked world as a sheet of music. Requires a World Lock (sold separately!).|0|4||0|||-1|-1||-1|-1||1||||||0||\nadd_button|farm_pack|`oFarm Pack``|interface/large/store_buttons/store_buttons5.rttex|`2You Get:`` 1 Cow, 1 Chicken, 10 Wheat, 10 Barn Block, 10 Red Wood Walls, 1 Barn Door, 1 Straw Hat, 1 Overalls, 1 Pitchfork, 1 Farmgirl Hair, 1 `#Rare`` `2Dear John Tractor``.<CR><CR>`5Description:`` Put the `2Grow`` in Growtopia with this pack, including a Cow you can milk, a Chicken that lays eggs and a farmer's outfit. Best of all? You get a `#Rare`` `2Dear John Tractor`` you can ride that will mow down trees!|0|4||0|||-1|-1||-1|-1||1||||||0||\nadd_button|school_pack|`oEducation Pack``|interface/large/store_buttons/store_buttons4.rttex|`2You Get:`` 10 ChalkBoards, 3 School Desks, 20 Red Bricks, 1 Bulletin Board, 10 Pencils, 1 Growtopia Lunchbox, 1 Grey Hair Bun, 1 Apple and 1 Random School Uniform Item.<CR><CR>`5Description:`` If you want to build a school in Growtopia, here's what you need!|0|6||0|||-1|-1||-1|-1||1||||||0||\nadd_button|home_pack|`oHome Pack``|interface/large/store_buttons/store_buttons9.rttex|`2You Get:`` 1 Television, 4 Couches, 2 Curtains, 1 Wall Clock, 1 Microwave, 1 Meaty Apron, 1 Ducky Pants, 1 Ducky top and 1 Eggs Benedict.<CR><CR>`5Description:`` Welcome home to Growtopia! Decorate with a Television, Window Curtains, Couches, a `#Rare`` Wall Clock that actually tells time, and a Microwave to cook in. Then dress up in a Meaty Apron and Ducky Pajamas to sit down and eat Eggs Benedict, which increases the amount of XP you earn!|0|0||0|||-1|-1||-1|-1||1||||||0|");
				}
				if (cch.find("action|info") == 0)
				{
					std::stringstream ss(cch);
					std::string to;
					int id = -1;
					int count = -1;
					while (std::getline(ss, to, '\n')) {
						vector<string> infoDat = explode("|", to);
						if (infoDat.size() == 3) {
							if (infoDat[1] == "itemID") id = atoi(infoDat[2].c_str());
							if (infoDat[1] == "count") count = atoi(infoDat[2].c_str());
						}
					}
					if (id == -1 || count == -1) continue;
					if (itemDefs.size() < id || id < 0) continue;
					packet::dialog(peer, "set_default_color|`o\n\nadd_label_with_icon|big|`w"+ itemDefs.at(id).name +"``|left|"+std::to_string(id)+"|\n\nadd_spacer|small|\nadd_textbox|"+ itemDefs.at(id).description +"|left|\nadd_spacer|small|\nadd_textbox|`oRarity:`w " + to_string(itemDefs.at(id).rarity) + "|left|\nadd_spacer|small||\nadd_textbox|`1This item cannot be spliced.|left|\n\nadd_spacer|small||\nadd_quick_exit|\nend_dialog|item_info|OK||");
				}
				if (cch.find("action|dialog_return") == 0)
				{
					std::stringstream ss(cch);
					std::string to;
					string btn = "";
					bool isRegisterDialog = false;
					bool isSDBDialog = false;
					bool isFindDialog = false;
					bool isWorldReportDialog = false;
					bool isBanPanel = false;
					bool isBeachDialog = false;
					string username = "";
					string password = "";
					string passwordverify = "";
					string email = "";
					string sdb1 = "";
					string sdb2 = "";
					string sdb3 = "";
					string ban_reason = "";
					int bantime = 0;
					string report_reason = "";
					string worldnamee = "";
					while (std::getline(ss, to, '\n')) {
						vector<string> infoDat = explode("|", to);
						if (infoDat.size() == 2) {
							if (infoDat[0] == "buttonClicked") btn = infoDat[1];
							if (infoDat[0] == "dialog_name" && infoDat[1] == "register")
							{
								isRegisterDialog = true;
								isSDBDialog = false;
								isWorldReportDialog = false;
								isBanPanel = false;
								isBeachDialog = false;
								isFindDialog = false;
							}else if (infoDat[0] == "dialog_name" && infoDat[1] == "super")
							{
								isSDBDialog = true;
								isRegisterDialog = false;
								isWorldReportDialog = false;
								isBanPanel = false;
								isBeachDialog = false;
								isFindDialog = false;
							}
							else if (infoDat[0] == "dialog_name" && infoDat[1] == "worldreport")
							{
								isSDBDialog = false;
								isRegisterDialog = false;
								isWorldReportDialog = true;
								isBanPanel = false;
								isBeachDialog = false;
								isFindDialog = false;
							}
							else if (infoDat[0] == "dialog_name" && infoDat[1] == "banpanel")
							{
								isSDBDialog = false;
								isRegisterDialog = false;
								isWorldReportDialog = false;
								isBanPanel = true;
								isBeachDialog = false;
								isFindDialog = false;
							}
							else if (infoDat[0] == "dialog_name" && infoDat[1] == "beach")
							{
								isSDBDialog = false;
								isRegisterDialog = false;
								isWorldReportDialog = false;
								isBanPanel = false;
								isBeachDialog = true;
								isFindDialog = false;
							}
							else if (infoDat[0] == "dialog_name" && infoDat[1] == "findid")
							{
								isSDBDialog = false;
								isRegisterDialog = false;
								isWorldReportDialog = false;
								isBanPanel = false;
								isBeachDialog = false;
								isFindDialog = true;
							}
							if (isRegisterDialog) {
								if (infoDat[0] == "username") username = infoDat[1];
								if (infoDat[0] == "password") password = infoDat[1];
								if (infoDat[0] == "passwordverify") passwordverify = infoDat[1];
								if (infoDat[0] == "email") email = infoDat[1];
							}else if (isSDBDialog) {
								if (infoDat[0] == "sdb1") sdb1 = infoDat[1];
								if (infoDat[0] == "sdb2") sdb2 = infoDat[1];
								if (infoDat[0] == "sdb3") sdb3 = infoDat[1];
							}
							else if (isWorldReportDialog) {
								if (infoDat[0] == "report_reason") report_reason = infoDat[1];
							}
							else if (isBanPanel) {
								if (infoDat[0] == "bantime") std::to_string(bantime) = infoDat[1];
								if (infoDat[0] == "banreason") ban_reason = infoDat[1];
							}
							else if (isBeachDialog) {
								if (infoDat[0] == "beachname") worldnamee = infoDat[1];
							}
							
							
						}
					}
					if (btn == "alist") {
						packet::dialog(peer, "set_default_color|`o|\nadd_label_with_icon|small|" + ((PlayerInfo*)(peer->data))->rawName + "'s Achievements|left|982||\nadd_spacer|small||\nadd_achieve|Builder (Classic)|Progress: 1%|left|127||\nadd_achieve|Farmer (Classic)|Progress: 2%|left|127||\nadd_achieve|Demolition (Classic)|Progress: 4%|left|127||\nadd_achieve|Packrat (Classic)|Progress: 7%|left|127||\nadd_achieve|Big Spender (Classic)|Progress: 0%|left|127||\nadd_achieve|Trashman (Classic)|Progress: 0%|left|127||\nadd_achieve|Paint The Town Blue (Classic)|Progress: 1%|left|127||\nadd_achieve|Embiggened (Classic)|Progress: 61%|left|127||\nadd_achieve|Ding! (Classic)|Progress: 20%|left|127||\nadd_achieve|Social Butterfly (Classic)|Progress: 9%|left|127||\nadd_achieve|You Like Me (Classic)|Not achieved!|left|127||\nadd_achieve|Milkin' It (Classic)|Progress: 0%|left|127||\nadd_achieve|Bubble Bubble (Classic)|Progress: 0%|left|127||\nadd_achieve|Surgeon (Classic)|Not achieved!|left|127||\nadd_achieve|Legen... (Classic)|Not achieved!|left|127||\nadd_achieve|Sparkly Dust (Classic)|Not achieved!|left|127||\nadd_achieve|Radiation Hunter (Classic)|Not achieved!|left|127||\nadd_achieve|Crimefighter (Classic)|Not achieved!|left|127||\nadd_achieve|Daily Challenge (Classic)|Not achieved!|left|127||\nadd_achieve|Deadly Vacuum|Progress: 0%|left|127||\nadd_achieve|Resurrection|Progress: 0%|left|127||\nadd_achieve|Who Ya Gonna Call?|Progress: 0%|left|127||\nadd_achieve|Mastermind|Progress: 0%|left|127||\nadd_achieve|Big Breaker!|Progress: 0%|left|127||\nadd_achieve|Hammer Time!|Progress: 0%|left|127||\nadd_achieve|Reaper King|Progress: 0%|left|127||\nadd_achieve|Bone Breaker|Progress: 0%|left|127||\nadd_achieve|Rod Snapper|Progress: 0%|left|127||\nadd_achieve|Trowel Troubles|Progress: 0%|left|127||\nadd_achieve|Cultivate This!|Progress: 0%|left|127||\nadd_achieve|Scrapped Scanners!|Progress: 0%|left|127||\nadd_achieve|Cooking Conundrum!|Progress: 0%|left|127||\nadd_achieve|Let The Right One In (Classic)|Not achieved!|left|126||\nadd_achieve|Wait for it... (Classic)|Progress: 0%|left|126||\nadd_achieve|Star Hoarder|Progress: 0%|left|126||\nadd_achieve|Bones!|Progress: 0%|left|126||\nadd_achieve|Ain't Afraid|Progress: 0%|left|126||\nadd_achieve|Matter Over Mind|Progress: 0%|left|126||\nadd_achieve|Growmoji Fan|Progress: 0%|left|126||\nadd_achieve|Bigger Breaker!|Progress: 0%|left|126||\nadd_achieve|Supporter (Classic)|Not achieved!|left|125||\nadd_achieve|King Of The World (Classic)|Not achieved!|left|125||\nadd_achieve|Superhero (Classic)|Not achieved!|left|125||\nadd_achieve|DARY! (Classic)|Progress: 0%|left|125||\nadd_achieve|Fishmaster (Classic)|Progress: 0%|left|125||\nadd_achieve|Tea, Earl Grey, Hot|Progress: 0%|left|125||\nadd_achieve|Runnin' on Empty|Progress: 0%|left|125||\nadd_achieve|Fin Fan|Progress: 0%|left|125||\nadd_achieve|Ghosted|Progress: 0%|left|125||\nadd_achieve|Growmoji Master|Progress: 0%|left|125||\nadd_achieve|Biggest Breaker!|Progress: 0%|left|125||\nadd_achieve|Guild Leader|Not achieved!|left|125||\nadd_achieve|Crafty|Progress: 0%|left|125||\nadd_achieve|Bright Future (Classic)|Not achieved!|left|124||\nadd_achieve|Harvester of Worlds (Classic)|Not achieved!|left|124||\nadd_achieve|Sacrifice (Classic)|Progress: 0%|left|124||\nadd_achieve|Costume Contest (Classic)|Not achieved!|left|124||\nadd_achieve|Spirit of Giving (Classic)|Progress: 0%|left|124||\nadd_achieve|Deer Hunter (Classic)|Not achieved!|left|124||\nadd_achieve|Celebrate Good Times (Classic)|Not achieved!|left|124||\nadd_achieve|Life of the Party (Classic)|Not achieved!|left|124||\nadd_achieve|First Birthday (Classic)|Progress: 0%|left|124||\nadd_achieve|Heartbreaker (Classic)|Not achieved!|left|124||\nadd_achieve|Stupid Cupid (Classic)|Not achieved!|left|124||\nadd_achieve|Four Leaves (Classic)|Not achieved!|left|124||\nadd_achieve|Little Green Man (Classic)|Not achieved!|left|124||\nadd_achieve|Sixteen Dozen (Classic)|Progress: 0%|left|124||\nadd_achieve|Bouncing Baby Bunny (Classic)|Not achieved!|left|124||\nadd_achieve|Bash Cinco (Classic)|Progress: 0%|left|124||\nadd_achieve|La Vida De La Fiesta (Classic)|Not achieved!|left|124||\nadd_achieve|The Last Celebration (Classic)|Not achieved!|left|124||\nadd_achieve|Summer Grillin' (Classic)|Not achieved!|left|124||\nadd_achieve|Growtopian Of The Year (Classic)|Not achieved!|left|124||\nadd_achieve|Crack Shot (Classic)|Not achieved!|left|124||\nadd_achieve|Disciple of Growganoth (Classic)|Not achieved!|left|124||\nadd_achieve|Concentrated Power of Will (Classic)|Not achieved!|left|124||\nadd_achieve|Seven Years Good Luck (Classic)|Not achieved!|left|124||\nadd_achieve|Ringu (Classic)|Not achieved!|left|124||\nadd_achieve|Wasted (Classic)|Not achieved!|left|124||\nadd_achieve|Death Racer (Classic)|Not achieved!|left|124||\nadd_achieve|Got Lucky Charms (Classic)|Not achieved!|left|124||\nadd_achieve|A Dead Rabbit (Classic)|Not achieved!|left|124||\nadd_achieve|Egg Hunter (Classic)|Progress: 0%|left|124||\nadd_achieve|The Brutalest Bounce (Classic)|Not achieved!|left|124||\nadd_achieve|Gorro (Classic)|Not achieved!|left|124||\nadd_achieve|Campeon (Classic)|Not achieved!|left|124||\nadd_achieve|Too Many Pineapples (Classic)|Progress: 0%|left|124||\nadd_achieve|Fresh Air (Classic)|Not achieved!|left|124||\nadd_achieve|Berserk|Progress: 0%|left|124||\nadd_achieve|The Special|Progress: 0%|left|124||\nadd_achieve|Crazy Farmer|Progress: 0%|left|124||\nadd_achieve|Millionaire|Progress: 0%|left|124||\nadd_achieve|Mad Shopper|Progress: 0%|left|124||\nadd_achieve|VOID|Progress: 0%|left|124||\nadd_achieve|Consumer|Progress: 0%|left|124||\nadd_achieve|SUPER SCIENCE!|Progress: 0%|left|124||\nadd_achieve|Providers Will Provide|Progress: 0%|left|124||\nadd_achieve|The Doctor|Progress: 0%|left|124||\nadd_achieve|Star Bling|Not achieved!|left|124||\nadd_achieve|Interstellar Champion|Not achieved!|left|124||\nadd_achieve|Showoff|Not achieved!|left|124||\nadd_achieve|Super Supporter|Not achieved!|left|124||\nadd_achieve|Chemical Creator|Progress: 0%|left|124||\nadd_achieve|Spiky Survivor|Not achieved!|left|124||\nadd_achieve|5th Anniversary|Not achieved!|left|124||\nadd_achieve|6th Anniversary|Not achieved!|left|124||\nadd_achieve|Wild Fire Savior|Not achieved!|left|124||\nadd_achieve|Big Donor|Progress: 0%|left|124||\nadd_achieve|Ancestral Being|Not achieved!|left|123||\nadd_achieve|Bossed Around|Not achieved!|left|123||\nadd_achieve|Master Farmer|Not achieved!|left|123||\nadd_achieve|Master Builder|Not achieved!|left|123||\nadd_achieve|Master Fisher|Not achieved!|left|123||\nadd_achieve|Master Chef|Not achieved!|left|123||\nadd_achieve|Master Surgeon|Not achieved!|left|123||\nadd_achieve|Master Star Captain|Not achieved!|left|123||\nadd_achieve|Chemical Crusader|Progress: 0%|left|123||\nadd_achieve|Vicious Victory|Progress: 0%|left|123||\nadd_achieve|Ascended|Not achieved!|left|123||\nadd_achieve|Descended|Not achieved!|left|123||\nadd_achieve|A Higher Power|Progress: 0%|left|122||\nadd_achieve|Boss Ghost: Toast|Progress: 0%|left|122||\nadd_achieve|Angel of Mercy|Not achieved!|left|122||\nadd_achieve|Ascended Universe|Progress: 0%|left|122||\nadd_achieve|Descended Universe|Progress: 0%|left|122||\nadd_achieve|Power Overwhelming|Progress: 0%|left|121||\nadd_achieve|Jack of All Trades|Not achieved!|left|121||\nadd_achieve|Big Showoff|Not achieved!|left|121||\nadd_achieve|Medical Marvel|Progress: 0%|left|121||\nadd_achieve|Interstellar Tycoon|Not achieved!|left|121||\nadd_achieve|Mine, All Mine (Classic)|Earned for using a lock.|left|24||\nadd_achieve|This Land Is My Land (Classic)|Earned for using a World Lock or Signal Jammer.|left|26||\nadd_spacer|small||\nend_dialog|alist||Back||\nadd_quick_exit|");
					}
					if (btn == "bruh" || btn == "small" || btn == "text" || btn == "buttons" || btn == "left" || btn == "738") {
						
						packet::consolemessage(peer, "`oHello there!");
						gamepacket_t p2;
						p2.Insert("OnConsoleMessage");
						p2.Insert("`5Hello there!");
						p2.CreatePacket(peer);
					}
					
					if (btn == "socialportal") {
						packet::dialog(peer, "set_default_color|`o|\nadd_label_with_icon|big| `wSocial Portal`` |left|1366||\nadd_spacer|small||\nadd_button|showfriend|`wShow Friends``|noflags|0|0||\nadd_button|show_apprentices|`wShow Apprentices``|noflags|0|0||\nadd_button|showguild|`wCreate Guild``|noflags|0|0||\nadd_spacer|small||\nadd_button|back|Back|noflags|0|0||\nadd_quick_exit||\nend_dialog|friends_guilds|||");
					}
					if (btn == "gazette") {
						packet::dialog(peer, newslist);
					}
					if (btn == "showfriend") {
						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 500 || ((PlayerInfo*)(peer->data))->adminLevel > 500) {
							packet::dialog(peer, "set_default_color|`o|\nadd_label_with_icon|big|0 of 6 `wFriends Online``|left|1366||\nadd_spacer|small||\nadd_textbox|`oNone of your friends are currently online.``|left||\nadd_spacer|small||\nadd_spacer|small||\nadd_button|friend_all|Show offline and ignored too|noflags|0|0||\nadd_button|all_friends|Edit Friends|noflags|0|0||\nadd_button|friends_options|Friend Options|noflags|0|0||\nadd_button|back|Back|noflags|0|0||\nadd_button||Close|noflags|0|0||\nend_dialog|friends||||\nadd_quick_exit|");
						}
						else if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) < 499 || ((PlayerInfo*)(peer->data))->adminLevel < 499) {
							packet::dialog(peer, "set_default_color|`o|\nadd_label_with_icon|big|0 of 0 `wFriends Online``|left|1366||\nadd_spacer|small||\nadd_textbox|`oNone of your friends are currently online.``|left||\nadd_spacer|small||\nadd_spacer|small||\nadd_button|friend_all|Show offline and ignored too|noflags|0|0||\nadd_button|all_friends|Edit Friends|noflags|0|0||\nadd_button|friends_options|Friend Options|noflags|0|0||\nadd_button|back|Back|noflags|0|0||\nadd_button||Close|noflags|0|0||\nend_dialog|friends||||\nadd_quick_exit|");
						}
					}
					if (btn == "friend_all") {
						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 500 || ((PlayerInfo*)(peer->data))->adminLevel > 500) {
							packet::dialog(peer, "set_default_color|`o|\nadd_label_with_icon|big|0 of 6 `wFriends Online``|left|1366||\nadd_spacer|small||\nadd_friend_image_label_button|4435701|`4(2h) `6@Matty``|game/tiles_page14.rttex|1.6|31|23|32|false|false||\nadd_friend_image_label_button|4435702|`4(2h) `6@Seth``|game/tiles_page14.rttex|1.6|31|23|32|false|false||\nadd_friend_image_label_button|4435703|`4(2h) `6@Hamumu``|game/tiles_page14.rttex|1.6|31|23|32|false|false||\nadd_friend_image_label_button|4435704|`4(2h) `6@Ubidev``|game/tiles_page14.rttex|1.6|31|23|32|false|false||\nadd_friend_image_label_button|4435705|`4(2h) `#@calvin``|game/tiles_page14.rttex|1.6|31|23|32|false|false||\nadd_friend_image_label_button|4435706|`4(2h) `#@YoMattty``|game/tiles_page14.rttex|1.6|31|23|32|false|false||\nadd_spacer|small||\nadd_textbox|`oYou aren't currently ignoring anyone.``|left||\nadd_spacer|small||\nadd_button|all_friends|Edit Friends|noflags|0|0||\nadd_button|friends_options|Friend Options|noflags|0|0||\nadd_button|back|Back|noflags|0|0||\nadd_button||Close|noflags|0|0||\nend_dialog|friends||||\nadd_quick_exit|");
						}
						else if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) < 499 || ((PlayerInfo*)(peer->data))->adminLevel < 499) {
							gamepacket_t p;
							p.Insert("OnTextOverlay");
							p.Insert("`wFriends has not been fully completed yet!");
							p.CreatePacket(peer);
							packet::dialog(peer, "set_default_color|`o|\nadd_label_with_icon|big|0 of 0 `wFriends Online``|left|1366||\nadd_spacer|small||\nadd_textbox|`oNone of your friends are currently online.``|left||\nadd_spacer|small||\nadd_spacer|small||\nadd_button|friend_all|Show offline and ignored too|noflags|0|0||\nadd_button|all_friends|Edit Friends|noflags|0|0||\nadd_button|friends_options|Friend Options|noflags|0|0||\nadd_button|back|Back|noflags|0|0||\nadd_button||Close|noflags|0|0||\nend_dialog|friends||||\nadd_quick_exit|");
						}
					}
					if (btn == "locks_menu") {
						packet::storerequest(peer, "set_description_text|Welcome to the `2Growtopia Store``! Select the item you'd like more info on.`o. `wWant to get `5Supporter`` status? Any Gem purchase (or`57,000`` Gems earned with free tapjoy`` offers) will make you one. You'll get new skin colors, the `5Recycle`` tool to convert unwanted items into Gems, and more bonuses!|\nenable_tabs|1|\nadd_tab_button|main_menu|Home|interface/large/btn_shop.rttex||0|0|0|0||||-1|-1|||0|\nadd_tab_button|locks_menu|Locks and Stuff|interface/large/btn_shop.rttex||1|1|0|0||||-1|-1|||0|\nadd_tab_button|itempack_menu|Item packs|interface/large/btn_shop.rttex||0|3|0|0||||-1|-1|||0|\nadd_tab_button|bigitems_menu|Awesome Items|interface/large/btn_shop.rttex||0|4|0|0||||-1|-1|||0|\nadd_tab_button|weather_menu|Weather Machines|interface/large/btn_shop.rttex|Tired of the same sunny sky? We offer alternatives within...|0|5|0|0||||-1|-1|||0|\nadd_tab_button|token_menu|Growtoken items|interface/large/btn_shop.rttex||0|2|0|0||||-1|-1|||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|1|\nadd_button|itemomonth|`oItem Of The Month``|interface/large/store_buttons/store_buttons16.rttex|April 2021:`` `9Grow X Rocket Wings``!<CR><CR>`2You Get:`` 1 `9Grow X Rocket Wings``.<CR><CR>`5Description:`` Take off on a mission to Mars, or wherever your imagination takes you! These rocket wings use the latest in renewable energy, are kind to the environment, and approved by Growtech for commercial space flight!|0|3|350000|0||interface/large/gui_store_button_overlays1.rttex|0|0||-1|-1||1||||||0|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|2|\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|0|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|3|\nadd_big_banner|interface/large/gui_store_iap_message.rttex|0|1|`oYou want to buy gems?! Great! But you'll need to add a `2GrowID`` to this account first, to make sure you won't lose what you buy if you drop your device in the toilet. It's free!|");
					}
					if (btn == "storebutton") {
						packet::storepurchaseresult(peer, "`oUh oh spagetthios!");
					}
					if (btn == "legendaryitems") {
						packet::dialog(peer, "set_default_color|`o\n\nadd_label_with_icon|big|`9Legendary Items!|left|32|\n\nadd_spacer|small|\nadd_label_with_icon|small|`9Legendary Title|left|1794|\nadd_button|buylname|`wPurchase`9 Legendary Title`w!|\nadd_label_with_icon|small|`1Legendbot-009|left|1780|\nadd_button|buylegenbot|`wPurchase`1 Legendbot-009`w!||\nadd_label_with_icon|small|`4Dragon of Legend|left|1782|\nadd_button|buyldrag|`wPurchase`4 Dragon of Legend`w!||\nadd_label_with_icon|small|`eLegendary Wings|left|1784|\nadd_button|buylwings|`wPurchase`e Legendary Wings`w!||\nadd_label_with_icon|small|`9Legendary Katana|left|2592|\nadd_button|buylkatana|`wPurchase`9 Legendary Katana`w!||\nadd_label_with_icon|small|`9Whip of Truth|left|6026|\nadd_button|buylwhip|`wPurchase`9 Whip of Truth`w!||\nadd_label_with_icon|small|`9Legendary Dragon Knight's Wings|left|7734|\nadd_button|buylknight|`wPurchase`9 Legendary Dragon Knight's Wings`w!||\nend_dialog|shoper|Cancel|\n");
					}
					if (btn == "buygems") {
						packet::dialog(peer, "set_default_color|`o\n\nadd_label_with_icon|big|`2Purchase Gems!|left|9438|\n\nadd_spacer|small|\nadd_textbox|`wPurchase in game gems for Growtopia `9world locks`w.|\nend_dialog|shoper|Cancel|\n");
					}
					if (btn == "buylname") {
						int price = 1000000;
						if (((PlayerInfo*)(peer->data))->hasLegenName == false) {
							if (((PlayerInfo*)(peer->data))->gems < price) {
								gamepacket_t p;
								p.Insert("OnTextOverlay");
								p.Insert("`wYou don't have enough gems for `9Legendary title`w!");
								p.CreatePacket(peer);
							}
							else {
								((PlayerInfo*)(peer->data))->gems = ((PlayerInfo*)(peer->data))->gems - price;
								((PlayerInfo*)(peer->data))->hasLegenName = true;
								((PlayerInfo*)(peer->data))->displayName = "`9" + ((PlayerInfo*)(peer->data))->rawName + " of Legend";
								sendState(peer);
								packet::consolemessage(peer, "`wYou have successfully purchased`9 Legendary Title!");
								gamepacket_t p1;
								p1.Insert("OnSetBux");
								p1.Insert(((PlayerInfo*)(peer->data))->gems);
								p1.CreatePacket(peer);
								sendSaveSet(peer);//should update the user's file AGAIN
							}
						}
						else {
							gamepacket_t p;
							p.Insert("OnTextOverlay");
							p.Insert("`wYou already have `9Legendary Title`w!");
							p.CreatePacket(peer);

						}
						
					}
					if (btn == "buylegenbot") {
						int price = 1000000;
						if (((PlayerInfo*)(peer->data))->hasLegenBot == false) {
							if (((PlayerInfo*)(peer->data))->gems < price) {
								gamepacket_t p;
								p.Insert("OnTextOverlay");
								p.Insert("`wYou don't have enough gems for `9Legendary title`w!");
								p.CreatePacket(peer);
							}
							else {
								((PlayerInfo*)(peer->data))->gems = ((PlayerInfo*)(peer->data))->gems - price;
								((PlayerInfo*)(peer->data))->hasLegenBot = true;
								sendState(peer);
								packet::consolemessage(peer, "`wYou have successfully purchased`9 Legendbot-009!");
								gamepacket_t p1;
								p1.Insert("OnSetBux");
								p1.Insert(((PlayerInfo*)(peer->data))->gems);
								p1.CreatePacket(peer);
								sendSaveSet(peer);//should update the user's file AGAIN
							}
						}
						else {
							gamepacket_t p;
							p.Insert("OnTextOverlay");
							p.Insert("`wYou already have `9Legendbot-009`w!");
							p.CreatePacket(peer);

						}

					}
					if (btn == "buyldrag") {
						int price = 1000000;
						if (((PlayerInfo*)(peer->data))->hasLegenDrag == false) {
							if (((PlayerInfo*)(peer->data))->gems < price) {
								gamepacket_t p;
								p.Insert("OnTextOverlay");
								p.Insert("`wYou don't have enough gems for `9Legendary Dragon`w!");
								p.CreatePacket(peer);
							}
							else {
								((PlayerInfo*)(peer->data))->gems = ((PlayerInfo*)(peer->data))->gems - price;
								((PlayerInfo*)(peer->data))->hasLegenDrag = true;
								sendState(peer);
								packet::consolemessage(peer, "`wYou have successfully purchased`9 Legendary Dragon!");
								gamepacket_t p1;
								p1.Insert("OnSetBux");
								p1.Insert(((PlayerInfo*)(peer->data))->gems);
								p1.CreatePacket(peer);
								sendSaveSet(peer);//should update the user's file AGAIN
							}
						}
						else {
							gamepacket_t p;
							p.Insert("OnTextOverlay");
							p.Insert("`wYou already have `9Legendary Dragon`w!");
							p.CreatePacket(peer);

						}

					}
					if (btn == "buylwings") {
						if (((PlayerInfo*)(peer->data))->hasLegenWing == true) {
							packet::consolemessage(peer, "`oWell you already have legendary wings!");
							cout << "true \n";
						}
						else if (((PlayerInfo*)(peer->data))->hasLegenWing == false) {
							packet::consolemessage(peer, "`oWell you don't have legendary wings!");
							cout << "false \n";
						}
					}
					/*if (btn == "buylwings") {
						bool wow = ((PlayerInfo*)(peer->data))->hasLegenWing;
						cout << "\n uh " + std::to_string(wow) + "\n";
						if (((PlayerInfo*)(peer->data))->hasLegenWing = false) {
							gamepacket_t p34;
							p34.Insert("OnConsoleMessage");
							p34.Insert("`oThis works?");
							p34.CreatePacket(peer);
					}
						if (((PlayerInfo*)(peer->data))->hasLegenWing = false) {
							int price = 1000000;
							int calc = price - 1;
							if (((PlayerInfo*)(peer->data))->gems < price) {
								gamepacket_t p;
								p.Insert("OnTextOverlay");
								p.Insert("`wYou don't have enough gems for `9Legendary Wings`w!");
								p.CreatePacket(peer);
							}
							else if (((PlayerInfo*)(peer->data))->gems > calc || ((PlayerInfo*)(peer->data))->gems == price){
								((PlayerInfo*)(peer->data))->gems = ((PlayerInfo*)(peer->data))->gems - price;
								((PlayerInfo*)(peer->data))->hasLegenWing = true;
								sendState(peer);
								packet::consolemessage(peer, "`wYou have successfully purchased`9 Legendary Wings!");
								gamepacket_t p1;
								p1.Insert("OnSetBux");
								p1.Insert(((PlayerInfo*)(peer->data))->gems);
								p1.CreatePacket(peer);
								sendSaveSet(peer);//should update the user's file AGAIN
							}
						}
						else if (((PlayerInfo*)(peer->data))->hasLegenWing = true) {
							gamepacket_t p;
							p.Insert("OnTextOverlay");
							p.Insert("`wYou already have `9Legendary Wings`w!");
							p.CreatePacket(peer);

						}

					}*/
					if (btn == "2week") {
						gamepacket_t p3;
						p3.Insert("OnTextOverlay");
						p3.Insert("``Successfully banned player for 2 weeks.");
						p3.CreatePacket(peer);
						gamepacket_t p4;
						p4.Insert("OnConsoleMessage");
						p4.Insert("`oNOTE! User wasn't actually banned, feature has not been added yet.");
						p4.CreatePacket(peer);
					}
					if (btn == "8week") {
						gamepacket_t p3;
						p3.Insert("OnTextOverlay");
						p3.Insert("``Successfully banned player for 8 weeks.");
						p3.CreatePacket(peer);
						gamepacket_t p4;
						p4.Insert("OnConsoleMessage");
						p4.Insert("`oNOTE! User wasn't actually banned, feature has not been added yet.");
						p4.CreatePacket(peer);
					}
					if (btn == "2week") {
						gamepacket_t p3;
						p3.Insert("OnTextOverlay");
						p3.Insert("``Successfully banned player for 2 weeks.");
						p3.CreatePacket(peer);
						gamepacket_t p4;
						p4.Insert("OnConsoleMessage");
						p4.Insert("`oNOTE! User wasn't actually banned, feature has not been added yet.");
						p4.CreatePacket(peer);
					}
					if (btn == "4week") {
						gamepacket_t p3;
						p3.Insert("OnTextOverlay");
						p3.Insert("``Successfully banned player for 4 weeks.");
						p3.CreatePacket(peer);
						gamepacket_t p4;
						p4.Insert("OnConsoleMessage");
						p4.Insert("`oNOTE! User wasn't actually banned, feature has not been added yet.");
						p4.CreatePacket(peer);
					}
					if (btn == "permaalt") {
						gamepacket_t p3;
						p3.Insert("OnTextOverlay");
						p3.Insert("``Successfully permanetly banned user.");
						p3.CreatePacket(peer);
						gamepacket_t p4;
						p4.Insert("OnConsoleMessage");
						p4.Insert("`oNOTE! User wasn't actually banned, feature has not been added yet.");
						p4.CreatePacket(peer);
					}
					if (btn == "2weekalt") {
						gamepacket_t p3;
						p3.Insert("OnTextOverlay");
						p3.Insert("``Successfully banned player for 2 weeks for bad stuff on alt(s).");
						p3.CreatePacket(peer);
						gamepacket_t p4;
						p4.Insert("OnConsoleMessage");
						p4.Insert("`oNOTE! User wasn't actually banned, feature has not been added yet.");
						p4.CreatePacket(peer);
					}
					if (btn == "permahacker") {
						gamepacket_t p3;
						p3.Insert("OnTextOverlay");
						p3.Insert("``Successfully permanetly banned user for hacking.");
						p3.CreatePacket(peer);
						gamepacket_t p4;
						p4.Insert("OnConsoleMessage");
						p4.Insert("`oNOTE! User wasn't actually banned, feature has not been added yet.");
						p4.CreatePacket(peer);
					}
					if (btn == "fakeauto") {
						gamepacket_t p3;
						p3.Insert("OnTextOverlay");
						p3.Insert("``Successfully sent a fake auto ban to user.");
						p3.CreatePacket(peer);
						gamepacket_t p4;
						p4.Insert("OnConsoleMessage");
						p4.Insert("`oNOTE! User wasn't actually banned, feature has not been added yet.");
						p4.CreatePacket(peer);
					}
					if (btn == "warpuser") {
						ENetPeer * currentPeer;
						gamepacket_t p4;
						p4.Insert("OnConsoleMessage");
						p4.Insert("`oNOTE! Wot");
						p4.CreatePacket(peer);
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							string name = ((PlayerInfo*)(currentPeer->data))->rawName;
							int pullX = ((PlayerInfo*)(peer->data))->x;
							int pullY = ((PlayerInfo*)(peer->data))->y;
							if (name == ((PlayerInfo*)(peer->data))->lastInfo)
							{
								string wrldname = ((PlayerInfo*)(currentPeer->data))->currentWorld;
								sendPlayerToWorld(peer, (PlayerInfo*)(peer->data), wrldname);
								WorldInfo info = worldDB.get(wrldname);
								sendWorld(peer, &info);
							}
						}
						
					}
					if (btn == "trade") {
						ENetPeer * currentPeer;
						
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							string name = ((PlayerInfo*)(currentPeer->data))->rawName;


							if (name == ((PlayerInfo*)(peer->data))->lastInfo)
							{
								gamepacket_t p1;
								p1.Insert("OnStartTrade");
								p1.Insert(name);
								p1.Insert(((PlayerInfo*)(currentPeer->data))->netID);
								p1.CreatePacket(currentPeer);

								gamepacket_t p3;
								p3.Insert("OnStartTrade");
								p3.Insert(((PlayerInfo*)(peer->data))->rawName);
								p3.Insert(((PlayerInfo*)(peer->data))->netID);
								p3.CreatePacket(peer);

								gamepacket_t p34;
								p34.Insert("OnTradeStatus");
								p34.Insert(((PlayerInfo*)(peer->data))->netID);
								p34.Insert(0);
								p34.Insert("`o" + ((PlayerInfo*)(peer->data))->rawName + "``'s offer.");
								p34.Insert("locked|0\naccepted|1");
								p34.CreatePacket(currentPeer);
								
								gamepacket_t p43;
								p34.Insert("OnTradeStatus");
								p34.Insert(((PlayerInfo*)(peer->data))->netID);
								p34.Insert(0);
								p34.Insert("`o" + ((PlayerInfo*)(peer->data))->rawName + "``'s offer.");
								p34.Insert("locked|0\nreset_locks|1\naccepted|0");
								p34.CreatePacket(currentPeer);

								gamepacket_t p4;
								p4.Insert("OnTradeStatus");
								p4.Insert(((PlayerInfo*)(currentPeer->data))->netID);
								p4.Insert(0);
								p4.Insert("`o" + ((PlayerInfo*)(currentPeer->data))->rawName + "``'s offer.");
								p4.Insert("locked|0\nreset_locks|1\naccepted|0");
								p4.CreatePacket(peer);


							}
						}

					}
					if (btn == "freeze") {
						ENetPeer * currentPeer;


						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							string name = ((PlayerInfo*)(currentPeer->data))->rawName;
							

							if (name == ((PlayerInfo*)(peer->data))->lastInfo)
							{
								if (((PlayerInfo*)(currentPeer->data))->isFrozen == false) {
									((PlayerInfo*)(currentPeer->data))->isFrozen = true;
									packet::consolemessage(currentPeer, "`oFreeze! (`$Frozen`o mod added)");
									sendState(peer);
									sendState(currentPeer);
								}
								else {
									((PlayerInfo*)(currentPeer->data))->isFrozen = false;
									packet::consolemessage(currentPeer, "`oYou've thawed out. (`$Frozen`o mod removed)");
									sendState(peer);
									sendState(currentPeer);
								}
							}
						}
					}
					if (btn == "showInfo") {
						ENetPeer * currentPeer;
						string online = "offline";
					
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							string name = ((PlayerInfo*)(currentPeer->data))->rawName;
							int pullX = ((PlayerInfo*)(peer->data))->x;
							int pullY = ((PlayerInfo*)(peer->data))->y;

							if (name == ((PlayerInfo*)(peer->data))->lastInfo)
							{
								string notes = "No user notes yet.";
								if (((PlayerInfo*)(currentPeer->data))->isIn == true) {
									string online = "online";

								}
								else if (((PlayerInfo*)(currentPeer->data))->isIn == false) {
									string online = "offline";
								}
								if (((PlayerInfo*)(currentPeer->data))->playerWarnings == 0) {
									notes = "No user notes yet.";
								}
								else if (((PlayerInfo*)(currentPeer->data))->playerWarnings > 0) {
									notes = "Player Warnings: `w" + std::to_string(((PlayerInfo*)(currentPeer->data))->playerWarnings)+ "`o.";
								}
								string name = ((PlayerInfo*)(currentPeer->data))->displayName;
								string rawname = ((PlayerInfo*)(currentPeer->data))->rawName;
								string growIP = ((PlayerInfo*)(currentPeer->data))->charIP;
								string countryID = ((PlayerInfo*)(currentPeer->data))->country;
								string requestID = ((PlayerInfo*)(currentPeer->data))->requestedName;
								int grownetID = ((PlayerInfo*)(currentPeer->data))->netID;
								//int grownetID = ((PlayerInfo*)(currentPeer->data))->char;
								//string warningsid = std::to_string(((PlayerInfo*)(currentPeer->data))->playerWarnings);
								//string playerWarn = std::to_string(((PlayerInfo*)(currentPeer->data))->playerWarnings);
								string playerWarn = std::to_string(((PlayerInfo*)(currentPeer->data))->playerWarnings);
								//packet::dialog(peer, "set_default_color|`o\nadd_label_with_icon|big|Showing info for: " + rawname + "|left|18||`o\n\nadd_label_with_icon|small|`oWarnings: `4" + playerWarn + "`o warnings total.|left|732|\nadd_label_with_icon|small|`oUser's Net ID: `5" + std::to_string(grownetID) + "`o.|left|5016|\nadd_label_with_icon|small|`oUser's IP: `5" + growIP + "`o.|left|5016|\nadd_label_with_icon|small|`oUser's Display Name: `5" + name + "`o.|left|5016|\nadd_label_with_icon|small|`oCountry: `5" + countryID + "`o.|left|5016||\nadd_label_with_icon|small|`oUser's Guest account: `5" + requestID + "`o.|left|5016|\nadd_spacer|small|\n\nadd_quick_exit|\nend_dialog|player_info||Close|");
								packet::dialog(peer, "add_label_with_icon|small|`wEditing " + ((PlayerInfo*)(currentPeer->data))->rawName + " (" + ((PlayerInfo*)(currentPeer->data))->requestedName + ") - #UNIQUEUSERID|left|274||\n\nadd_label_with_icon|small|`o" + online +". `#" + ((PlayerInfo*)(currentPeer->data))->rawName + " `w(" + ((PlayerInfo*)(currentPeer->data))->requestedName + ") UNIQUEUSERID `oHrs: HOURS IP: " + ((PlayerInfo*)(currentPeer->data))->charIP + "|left|658|\nadd_button|warpuser|`oWarp To User (in`# " + ((PlayerInfo*)(currentPeer->data))->currentWorld + "`o)|\n\n\nadd_spacer|small|\nadd_textbox|`oAccount notes:|\nadd_textbox|`oNo user notes yet.|\n\nadd_spacer|small|\nadd_label_with_icon_button|small|`o<-- 2 week (" + ((PlayerInfo*)(currentPeer->data))->rawName + ")|left|732|2week||\nadd_label_with_icon_button|small|`o<-- 4 week (" + ((PlayerInfo*)(currentPeer->data))->rawName + ")|left|732|4week||\nadd_label_with_icon_button|small|`o<-- Ban Panel (" + ((PlayerInfo*)(currentPeer->data))->rawName + ")|left|732|banpanels||\nadd_label_with_icon_button|small|`o<-- 8 week (" + ((PlayerInfo*)(currentPeer->data))->rawName + ")|left|732|8week||\nadd_label_with_icon_button|small|`o<-- Perma ban (" + ((PlayerInfo*)(currentPeer->data))->rawName + ")|left|732|permaban||\nadd_label_with_icon_button|small|`o<-- Perma ban for bad stuff on alt(s)|left|732|permaalt||\nadd_label_with_icon_button|small|`o<-- 2 week ban for bad stuff on alt(s)|left|732|2weekalt||\nadd_label_with_icon_button|small|`o<-- Perma ban for hacker|left|732|permahacker||\nadd_spacer|small|\nadd_label_with_icon_button|small|`o<-- Fake auto-ban (use for hackers, confuses them, online only)|left|1908|fakeauto||\n\nadd_spacer|small||\n\nend_dialog|gazette|Continue|\n");
							}

						}
						
					}
					if (btn == "banpanels") {
						ENetPeer * currentPeer;
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							string name = ((PlayerInfo*)(currentPeer->data))->rawName;
							if (name == ((PlayerInfo*)(peer->data))->lastInfo)
							{
								string dname = ((PlayerInfo*)(currentPeer->data))->displayName;
								string rname = ((PlayerInfo*)(currentPeer->data))->rawName;
								packet::dialog(peer, "add_label_with_icon|small|`eBanning Pannel|left|732|\nadd_textbox|`ePlayer: " + dname + "`e(`w" + rname +"`e)|\nadd_textbox|`eKeep in mind that a ban that equals or is more than `4730`e days will change the ban to a `4suspension`e.|\nadd_textbox|`eEnter an `4appropriate`e ban time and reason.|\nadd_text_input|bantime|`eBan Time:||3||\nadd_text_input|banreason|`eReason:||30|\nadd_quick_exit|\nend_dialog|banpanel|Cancel|OK|");
							}
						}
					}
					if (btn == "fakeauto") {
						ENetPeer * currentPeer;
						gamepacket_t pss;
						pss.Insert("OnTextOverlay");
						pss.Insert("`wSent Fake Auto Ban to player!");
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							string name = ((PlayerInfo*)(currentPeer->data))->rawName;
							if (name == ((PlayerInfo*)(peer->data))->lastInfo)
							{
								gamepacket_t p3;
								p3.Insert("OnConsoleMessage");
								p3.Insert("`#** `$The Ancient Ones`o have used `5Ban`o on `w" + name + " `#** `o(`$/rules`o to view the rules!)");
								p3.CreatePacket(currentPeer);
								gamepacket_t p73;
								p73.Insert("OnConsoleMessage");
								p73.Insert("`4** `$" + name + "`4 AUTO-BANNED BY SYSTEM ** `o(`$/rules `oto view rules)");
								p73.CreatePacket(currentPeer);
								string text = "action|play_sfx\nfile|audio/keypad_hit.wav\ndelayMS|0\n";
								BYTE* data = new BYTE[5 + text.length()];
								BYTE zero = 0;
								int type = 3;
								memcpy(data, &type, 4);
								memcpy(data + 4, text.c_str(), text.length());
								memcpy(data + 4 + text.length(), &zero, 1);
								ENetPacket * packet2 = enet_packet_create(data,
									5 + text.length(),
									ENET_PACKET_FLAG_RELIABLE);

								enet_peer_send(currentPeer, 0, packet2);
								gamepacket_t p83;
								p83.Insert("OnAddNotification");
								p83.Insert("interface/atomic_button.rttex");
								p83.Insert("`wWarning from `4System`w: You've been `4BANNED`w from Growtopia for 730 days");
								p83.Insert("audio/hub_open.wav");
								p83.CreatePacket(currentPeer);
								gamepacket_t p43;
								p43.Insert("OnConsoleMessage");
								p43.Insert("`oWarning from`4 System`o: You've been `4BANNED`o from Growtopia for 730 days");
								p43.CreatePacket(currentPeer);
								gamepacket_t p2;
								p2.Insert("OnConsoleMessage");
								p2.Insert("`oReality flickers as you begin to wake up. (`$Ban`o mod added, `$730 days`o left)");
								p2.CreatePacket(currentPeer);
								enet_peer_disconnect_later(currentPeer, 0);
							}
						}
					}
					if (btn == "permaban") {
						ENetPeer * currentPeer;
						packet::consolemessage(peer, "`otesting");
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							string name = ((PlayerInfo*)(currentPeer->data))->displayName;
							int pullX = ((PlayerInfo*)(peer->data))->x;
							int pullY = ((PlayerInfo*)(peer->data))->y;

							if (name == ((PlayerInfo*)(peer->data))->lastInfo)
							{
								PlayerInfo* p = ((PlayerInfo*)(currentPeer->data));
								std::ifstream od("players/" + name + ".json");
								if (od.is_open()) {
								}

								std::ofstream o("players/" + name + ".json");
								if (!o.is_open()) {
									cout << GetLastError() << endl;
									_getch();
								}
								json j;
								bool playerSuspended = true;

								j["isSuspended"] = playerSuspended;
								j["username"] = p->rawName;
								j["password"] = p->pactPass;
								j["email"] = p->tankIDEmail;
								j["adminLevel"] = p->adminLevel;
								j["playerWarnings"] = p->playerWarnings;
								j["ClothBack"] = p->cloth_back;
								j["ClothHand"] = p->cloth_hand;
								j["ClothFace"] = p->cloth_face;
								j["ClothShirt"] = p->cloth_shirt;
								j["ClothPants"] = p->cloth_pants;
								j["ClothNeck"] = p->cloth_necklace;
								j["ClothHair"] = p->cloth_hair;
								j["ClothFeet"] = p->cloth_feet;
								j["ClothMask"] = p->cloth_mask;
								j["hasLegenName"] = p->hasLegenName;
								j["hasLegenDrag"] = p->hasLegenDrag;
								j["hasLegenBot"] = p->hasLegenBot;
								j["hasLegenWing"] = p->hasLegenWing;
								j["hasLegenKatana"] = p->hasLegenKatana;
								j["hasLegenWhip"] = p->hasLegenWhip;
								j["hasLegenKnight"] = p->hasLegenKnight;
								o << j << std::endl;
								((PlayerInfo*)(currentPeer->data))->isSuspended = true;


								{
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("`2You have suspended the player.");
									p.CreatePacket(peer);

									gamepacket_t p2;
									p2.Insert("OnConsoleMessage");
									p2.Insert("`#** `$The Ancient Ones have`o `4banned`` `w" + name + "`#** `$(`4/rules`o to see the rules!)");
									string text = "action|play_sfx\nfile|audio/blank.wav\ndelayMS|0\n";
									BYTE* data = new BYTE[5 + text.length()];
									BYTE zero = 0;
									int type = 3;
									memcpy(data, &type, 4);
									memcpy(data + 4, text.c_str(), text.length());
									memcpy(data + 4 + text.length(), &zero, 1);

									gamepacket_t p3;
									p3.Insert("OnConsoleMessage");
									p3.Insert("`#** `$The Ancient Ones`o have used `5Ban`o on `w" + name + " `#** `o(`$/rules`o to view the rules!)");
									string text1 = "action|play_sfx\nfile|audio/already_used.wav\ndelayMS|0\n";
									BYTE* data1 = new BYTE[5 + text1.length()];
									BYTE zero1 = 0;
									int type1 = 3;
									memcpy(data1, &type1, 4);
									memcpy(data1 + 4, text1.c_str(), text1.length());
									memcpy(data1 + 4 + text1.length(), &zero1, 1);


									ENetPeer * currentPeer;

									for (currentPeer = server->peers;
										currentPeer < &server->peers[server->peerCount];
										++currentPeer)
									{
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
											continue;

										ENetPacket * packet3 = enet_packet_create(data,
											5 + text.length(),
											ENET_PACKET_FLAG_RELIABLE);

										p2.CreatePacket(currentPeer);



										enet_peer_send(currentPeer, 0, packet3);

										if (isHere(peer, currentPeer))
										{

											ENetPacket * packet5 = enet_packet_create(data1,
												5 + text1.length(),
												ENET_PACKET_FLAG_RELIABLE);
											p3.CreatePacket(currentPeer);
											enet_peer_send(currentPeer, 0, packet5);

										}
										//enet_host_flush(server);
									}


								}
								gamepacket_t p83;
								p83.Insert("OnAddNotification");
								p83.Insert("interface/atomic_button.rttex");
								p83.Insert("`wWarning from `4System`w: You've been `4BANNED`w from Growtopia for 730 days");
								p83.Insert("audio/hub_open.wav");
								p83.CreatePacket(currentPeer);
								gamepacket_t p43;
								p43.Insert("OnConsoleMessage");
								p43.Insert("`wWarning from`4 System`w: You've been `4BANNED`w from Growtopia for 730 days");
								p43.CreatePacket(currentPeer);
								gamepacket_t p2;
								p2.Insert("OnConsoleMessage");
								p2.Insert("`oReality flickers as you begin to wake up. (`$Ban`o mod added, `$730 days`o left)");
								p2.CreatePacket(currentPeer);
								enet_peer_disconnect_later(currentPeer, 0);
							}

						}
					}
					if (btn == "givecarnitickets") {
						gamepacket_t p;
						p.Insert("OnDialsogRequest");
						p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`9 Quest For The Ring|left|1900|\n\nadd_spacer|small||\nadd_textbox|`oYou are about to embark on a magical journey! It won't be easy, but the reward is one of the fine rings from my vast collection. You don't get to pick which one, mind you.\nI've got a lot of rings sitting in my bag, with a lotof magical properties. If you want one, picked randomly at my discretion, you'll need to complete 10 little tasks for me.\nDoesn't sound so bad does it?\nRemember that I travel with the carnival, so if you don't get your quest done this time, be sure to catch me again next month! There's one last thing you should know before you begin. You can quit your quest at any time, but be aware that if you do, you'll lose all your progress, and you'll have to pay another 10 Golden Tickets to start again!|left||\nadd_spacer|small||\nadd_textbox|`oSo... now that you've recieved the official disclaimer, are you truly prepared to hand over 10 Golden Tickets and embark on the Quest For The Ring?|big||left||\nadd_spacer|small|\nadd_quick_exit||noflags|0|0|\nnend_dialog|gazette|Goodbye!|OK|");
						p.CreatePacket(peer);
					}

					if (btn == "effect0")
					{
						((PlayerInfo*)(peer->data))->effect = -1; // punch
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);

					}
					if (btn == "effect1")
					{
						((PlayerInfo*)(peer->data))->effect = -500;	 // death ray	
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);

					}
					if (btn == "effect2")
					{
						((PlayerInfo*)(peer->data))->effect = 8421381; // shotgun
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect3")
					{
						((PlayerInfo*)(peer->data))->effect = 8420386; // Tank
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect4")
					{
						((PlayerInfo*)(peer->data))->effect = 8420396; // Silver Bow
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect5")
					{
						((PlayerInfo*)(peer->data))->effect = 8420389; // Rock Hammer
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect6")
					{
						((PlayerInfo*)(peer->data))->effect = 8421382; // Dragon
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect7")
					{
						((PlayerInfo*)(peer->data))->effect = -979; // egg
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect8")
					{
						((PlayerInfo*)(peer->data))->effect = -1017; // idk
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect9")
					{
						((PlayerInfo*)(peer->data))->effect = -991; // ice drag
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect10")
					{
						((PlayerInfo*)(peer->data))->effect = -1002; // Focused Eyes
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect11")
					{
						((PlayerInfo*)(peer->data))->effect = -991; // icedrag
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect12")
					{
						((PlayerInfo*)(peer->data))->effect = -997; // chaos curse wand
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect13")
					{
						((PlayerInfo*)(peer->data))->effect = 8420394;
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect14")
					{
						((PlayerInfo*)(peer->data))->effect = 8421391;
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect15")
					{
						((PlayerInfo*)(peer->data))->effect = 8421391; // battle trout
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect16")
					{
						((PlayerInfo*)(peer->data))->effect = -1008; // fiesta
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect17")
					{
						((PlayerInfo*)(peer->data))->effect = -1007; // squirt
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect18")
					{
						((PlayerInfo*)(peer->data))->effect = -1006; // key tar
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect19")
					{
						((PlayerInfo*)(peer->data))->effect = -940; // drag V2
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect20")
					{
						((PlayerInfo*)(peer->data))->effect = -1004; // lbot
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect21")
					{
						((PlayerInfo*)(peer->data))->effect = -1003; // l drag
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect22")
					{
						((PlayerInfo*)(peer->data))->effect = -950; // fiesta
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect23")
					{
						((PlayerInfo*)(peer->data))->effect = -996; // Green Swe
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect24")
					{
						((PlayerInfo*)(peer->data))->effect = -977; // Gungir
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect25")
					{
						((PlayerInfo*)(peer->data))->effect = -964; // Blue Soul
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect26")
					{
						((PlayerInfo*)(peer->data))->effect = -1011; //Black Shadow Effect
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);

					}
					if (btn == "effect27")
					{
						((PlayerInfo*)(peer->data))->effect = -992; // Black Crystal
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect28")
					{
						((PlayerInfo*)(peer->data))->effect = -981; // l katana
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect29")
					{
						((PlayerInfo*)(peer->data))->effect = -978; // Black Holes
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect30")
					{
						((PlayerInfo*)(peer->data))->effect = 8420397; // Pineapple Blur Effect
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "effect31")
					{
						((PlayerInfo*)(peer->data))->effect = -995; // digger spade
						sendState(peer); //here
						sendPuncheffectpeer(peer, ((PlayerInfo*)(peer->data))->effect);
					}
					if (btn == "worldPublic") if (((PlayerInfo*)(peer->data))->rawName == getPlyersWorld(peer)->owner) getPlyersWorld(peer)->isPublic = true;
					if(btn == "worldPrivate") if (((PlayerInfo*)(peer->data))->rawName == getPlyersWorld(peer)->owner) getPlyersWorld(peer)->isPublic = false;
#ifdef REGISTRATION
					if (isRegisterDialog) {

						int regState = PlayerDB::playerRegister(peer, username, password, passwordverify, email);
						if (regState == 1) {

							ENetPeer * currentPeer;
							//string rawname = ((PlayerInfo*)(currentPeer->data))->rawName;
							packet::consolemessage(peer, "`rYour account has been created!``");
							gamepacket_t p;
							p.Insert("SetHasGrowID");
							p.Insert(1);
							p.Insert(username);
							p.Insert(password);
							p.CreatePacket(peer);
							gamepacket_t p5;
							p5.Insert("OnDialogRequest");
							p5.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`wGrowID GET!``|left|206|\nadd_textbox|`oA `wGrowID`o with the log on of `w" + username + "`o and the password of`w " + password + "`o created. Write them down, they will be required to log on from now on!|\nend_dialog|growidgot|Continue||");
							p5.CreatePacket(peer);

							gamepacket_t p7(0, ((PlayerInfo*)(peer->data))->netID);
							p7.Insert("OnNameChanged");
							p7.Insert(username);

							((PlayerInfo*)(peer->data))->displayName = username;

							gamepacket_t p8;
							p8.Insert("OnNameChanged");
							p8.Insert(username);

							gamepacket_t p3;
							p3.Insert("OnConsoleMessage");
							p3.Insert("`2[`9Mod-Logs`2] A new account has been created with raw name  : `5" + username + "`2.");

							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;

								if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 495) {
									p3.CreatePacket(currentPeer);
									p7.CreatePacket(currentPeer);
									p8.CreatePacket(currentPeer);

									//enet_host_flush(server);
								}

							}

							//enet_peer_disconnect_later(peer, 0);
							
						}
						else if(regState==-1) {
							packet::consolemessage(peer, "`rAccount creation has failed, because it already exists!``");
						}
						else if (regState == -2) {
							packet::consolemessage(peer, "`rAccount creation has failed, because the name is too short!``");
						}
						else if (regState == -3) {
							packet::consolemessage(peer, "`4Passwords mismatch!``");
						}
						else if (regState == -4) {
							packet::consolemessage(peer, "`4Account creation has failed, because email address is invalid!``");
						}
						else if (regState == -5) {
							packet::consolemessage(peer, "`4Account creation has failed, because Discord ID is invalid!``");
						}
						else if (regState == -8) {
							packet::consolemessage(peer, "`4Account creation has failed, because username contains invalid characters.");
						}
						else if (regState == -9) {
							packet::consolemessage(peer, "`4Account creation has failed. Your username can't be that `9LEGENDARY!`4. :[");
						}
					}
					else if (isWorldReportDialog) {
						
						ENetPeer * currentPeer;
						string reason = report_reason;
						string reporter = ((PlayerInfo*)(peer->data))->rawName;
						string dreporter = ((PlayerInfo*)(peer->data))->displayName;
						string reporterworld = ((PlayerInfo*)(peer->data))->currentWorld;

						gamepacket_t p2;
						p2.Insert("OnConsoleMessage");
						p2.Insert("`9[`2MOD-LOGS`9] `4There was a world report from `2" + reporter + "`9(`2" + dreporter + "`9)`4, in world`2 " + reporterworld + "`4, the report reason was `2" + reason + "`4.");
						string text = "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0\n";
						BYTE* data = new BYTE[5 + text.length()];
						BYTE zero = 0;
						int type = 3;
						memcpy(data, &type, 4);
						memcpy(data + 4, text.c_str(), text.length());
						memcpy(data + 4 + text.length(), &zero, 1);

						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;

							if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 495 && getAdminLevel(((PlayerInfo*)(currentPeer->data))->rawName, ((PlayerInfo*)(currentPeer->data))->tankIDPass) > 495) {
								p2.CreatePacket(currentPeer);

								ENetPacket * packet2 = enet_packet_create(data,
									5 + text.length(),
									ENET_PACKET_FLAG_RELIABLE);

								enet_peer_send(currentPeer, 0, packet2);

								//enet_host_flush(server);
							}

						}
					}
					else if (isBanPanel) {
					//ENetPeer * peer;
					int times = bantime;
					string reaso = ban_reason;
					packet::consolemessage(peer, "`2[`9DEBUG`2]`9 Ban Reason: " + reaso + " Ban Time: " + std::to_string(times) + ".");
					}
					else if (isBeachDialog) {
					ifstream ifile;
					ifile.open("worlds/" + worldnamee + ".json");
					if (ifile) {
						packet::consolemessage(peer, "`oCannot create new world! World already exists! Use a unique world name instead.");
					}
					else {
						//create world
						if (worldnamee == "CON" || worldnamee == "PRN" || worldnamee == "AUX" || worldnamee == "NUL" || worldnamee == "COM1" || worldnamee == "COM2" || worldnamee == "COM3" || worldnamee == "COM4" || worldnamee == "COM5" || worldnamee == "COM6" || worldnamee == "COM7" || worldnamee == "COM8" || worldnamee == "COM9" || worldnamee == "LPT1" || worldnamee == "LPT2" || worldnamee == "LPT3" || worldnamee == "LPT4" || worldnamee == "LPT5" || worldnamee == "LPT6" || worldnamee == "LPT7" || worldnamee == "LPT8" || worldnamee == "LPT9" || worldnamee == "EXIT") {
							packet::consolemessage(peer, "`oCannot create world. World name uses invalid characters.");
						}
						else {
							toUpperCase(worldnamee);
							generateBeachWorld(worldnamee, 100, 60);
							packet::consolemessage(peer, "`oMagic! `5A New Beach`o is created!");
							ENetPeer * currentPeer;

							PlayerInfo* p = ((PlayerInfo*)(peer->data));

							sendPlayerToWorld(peer, (PlayerInfo*)(peer->data), worldnamee);
							WorldInfo info = worldDB.get(worldnamee);
							sendWorld(peer, &info);
							gamepacket_t p1;
							p1.Insert("OnTextOverlay");
							p1.Insert("`wMagically warping to `o" + worldnamee);
							p1.CreatePacket(peer);
						}
						
				       }
                     }
					else if (isSDBDialog) {
						ENetPeer * currentPeer;
						string first = sdb1;//same
						string second = sdb2;//idk
						string third = sdb3;//this one works
						string name = ((PlayerInfo*)(peer->data))->rawName;
						string dname = ((PlayerInfo*)(peer->data))->displayName;
						string world = ((PlayerInfo*)(peer->data))->currentWorld;
						gamepacket_t p333;
						p333.Insert("OnDialogRequest");
						//p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`2Super-Duper-Broadcast``|left|660|\n\nadd_spacer|small|\nadd_textbox|`oSuper Duper Broadcasts pop up a box with your message on `4every`o player's screen. The box even includes a button to visit your world!|\nadd_spacer||small|`oEnter up to 3 lines of text!``|\nadd_spacer|small|\nadd_text_input|SDB1|SDB Line 1||30|\nadd_text_input|SDB2|SDB Line 2|30|\nadd_text_input|SDB3|SDB Line 3||30|\nend_dialog|super|Cancel|Send SDB!|\n");
						p333.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`2Broadcast from " + dname + "|left|660|\nadd_smalltext|`o(In world "+ world +")|left|\nadd_textbox|" + sdb1 + "|\nadd_textbox|" + sdb2 + "|\nadd_textbox|" + sdb3 + "|\nend_dialog|gazette|Cancel|sdbgo|GO!||\n");
						time_t now = time(0);
						tm *ltm = localtime(&now);
						cout << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "]" << " A SDB was sent by " + ((PlayerInfo*)(peer->data))->rawName + ". Take action if this user isn't a developer.\n";
						gamepacket_t p3;
						p3.Insert("OnConsoleMessage");
						p3.Insert("`w** `2Super-DuperBroadcast`w from " + dname + " `w(in`5 JAMMED`w) ** `5:");
						gamepacket_t p4;
						p4.Insert("OnConsoleMessage");
						p4.Insert("`w**** " + first);
						gamepacket_t p5;
						p5.Insert("OnConsoleMessage");
						p5.Insert("`w**** " + second);
						gamepacket_t p6;
						p6.Insert("OnConsoleMessage");
						p6.Insert("`w**** " + third);
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							if (!((PlayerInfo*)(currentPeer->data))->radio)
								continue;
							p333.CreatePacket(currentPeer);
							p3.CreatePacket(currentPeer);
							p4.CreatePacket(currentPeer);
							p5.CreatePacket(currentPeer);
							p6.CreatePacket(currentPeer);
						}
					}
#endif
				}
				string dropText = "action|drop\n|itemID|";
				if (cch.find(dropText) == 0)
				{
					//sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft?-1:1)), ((PlayerInfo*)(peer->data))->y, atoi(cch.substr(dropText.length(), cch.length() - dropText.length() - 1).c_str()), 1, 0, false);
					sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y, atoi(cch.substr(dropText.length(), cch.length() - dropText.length() - 1).c_str()), 1, 0);
					/*int itemID = atoi(cch.substr(dropText.length(), cch.length() - dropText.length() - 1).c_str());
					PlayerMoving data;
					data.packetType = 14;
					data.x = ((PlayerInfo*)(peer->data))->x;
					data.y = ((PlayerInfo*)(peer->data))->y;
					data.netID = -1;
					data.plantingTree = itemID;
					float val = 1; // item count
					BYTE val2 = 0; // if 8, then geiger effect
					
					BYTE* raw = packPlayerMoving(&data);
					memcpy(raw + 16, &val, 4);
					memcpy(raw + 1, &val2, 1);
					SendPacketRaw(4, raw, 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);*/
				}
				
				if (cch.find("text|") != std::string::npos) {
					PlayerInfo* pData = ((PlayerInfo*)(peer->data));
					if (str == "/ghost")
					{
						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 499 || ((PlayerInfo*)(peer->data))->adminLevel > 499) {
							if (((PlayerInfo*)(peer->data))->canWalkInBlocks == false) {
								((PlayerInfo*)(peer->data))->canWalkInBlocks = true;
								sendState(peer);
								((PlayerInfo*)(peer->data))->skinColor = -155;
								sendClothes(peer);
								packet::consolemessage(peer, "`oYour atoms are suddenly aware of quantum tunneling. (Ghost in the shell mod added)");
								sendSaveSet(peer);
							}
							else if (((PlayerInfo*)(peer->data))->canWalkInBlocks == true) {
								((PlayerInfo*)(peer->data))->canWalkInBlocks = false;
								((PlayerInfo*)(peer->data))->skinColor = -2104114177;
								sendState(peer);
								sendClothes(peer);
								packet::consolemessage(peer, "`oYour body stops shimmering and returns to normal. (Ghost in the shell mod removed)");
								sendSaveSet(peer);
							}

						}
					}
					else if (str.substr(0, 5) == "/pay ") {
						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 495 || ((PlayerInfo*)(peer->data))->adminLevel > 495) {

							string msg_info = str;

							size_t extra_space = msg_info.find("  ");
							if (extra_space != std::string::npos) {
								msg_info.replace(extra_space, 2, " ");
							}

							string delimiter = " ";
							size_t pos = 0;
							string gem_reciever;
							string gem;
							if ((pos = msg_info.find(delimiter)) != std::string::npos) {
								msg_info.erase(0, pos + delimiter.length());
							}
							else {
								gamepacket_t ps4;
								ps4.Insert("OnConsoleMessage");
								ps4.Insert("`oAborting...");
								ps4.CreatePacket(peer);
								gamepacket_t ps;
								ps.Insert("OnConsoleMessage");
								ps.Insert("`oPlease enter a Player Name.");
								ps.CreatePacket(peer);
							}

							if ((pos = msg_info.find(delimiter)) != std::string::npos) {
								gem_reciever = msg_info.substr(0, pos);
								msg_info.erase(0, pos + delimiter.length());
							}
							else {
								gamepacket_t ps5;
								ps5.Insert("OnConsoleMessage");
								ps5.Insert("`oAborting...");
								ps5.CreatePacket(peer);
								gamepacket_t ps;
								ps.Insert("OnConsoleMessage");
								ps.Insert("`oThere is no warn message inputed.");
								ps.CreatePacket(peer);
							}

							gem = msg_info;
							ENetPeer* currentPeer;

							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;
								if (((PlayerInfo*)(currentPeer->data))->rawName == PlayerDB::getProperName(gem_reciever)) {
									if (((PlayerInfo*)(currentPeer->data))->gems >= stoi(gem)) {
										((PlayerInfo*)(currentPeer->data))->gems = ((PlayerInfo*)(currentPeer->data))->gems + stoi(gem);
										((PlayerInfo*)(peer->data))->gems = ((PlayerInfo*)(peer->data))->gems - stoi(gem);
										packet::setbux(currentPeer, ((PlayerInfo*)(currentPeer->data))->gems);
										packet::setbux(peer, ((PlayerInfo*)(peer->data))->gems);
										packet::consolemessage(peer, "`o`2GEMMIES!`o You sent `5" + gem + "`o Gems to `5" + ((PlayerInfo*)(currentPeer->data))->rawName + "`o!");
										packet::consolemessage(currentPeer, "`2GEMMIES!`o You recieved `5" + gem + "`o Gems from `5" + ((PlayerInfo*)(peer->data))->rawName + "`o!");
									}
									else if (((PlayerInfo*)(currentPeer->data))->gems < stoi(gem)) {
										packet::consolemessage(peer, "`oYou don't have enough gems!");
									}
									

								}

							}
						}
					}
					else if (str.substr(0,6) == "/warn ") {
						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 495 || ((PlayerInfo*)(peer->data))->adminLevel > 495) {

							string msg_info = str;

							size_t extra_space = msg_info.find("  ");
							if (extra_space != std::string::npos) {
								msg_info.replace(extra_space, 2, " ");
							}

							string delimiter = " ";
							size_t pos = 0;
							string warn_user;
							string pm_message;
							if ((pos = msg_info.find(delimiter)) != std::string::npos) {
								msg_info.erase(0, pos + delimiter.length());
							}
							else {
								gamepacket_t ps4;
								ps4.Insert("OnConsoleMessage");
								ps4.Insert("`oAborting...");
								ps4.CreatePacket(peer);
								gamepacket_t ps;
								ps.Insert("OnConsoleMessage");
								ps.Insert("`oPlease enter a Player Name.");
								ps.CreatePacket(peer);
							}

							if ((pos = msg_info.find(delimiter)) != std::string::npos) {
								warn_user = msg_info.substr(0, pos);
								msg_info.erase(0, pos + delimiter.length());
							}
							else {
								gamepacket_t ps5;
								ps5.Insert("OnConsoleMessage");
								ps5.Insert("`oAborting...");
								ps5.CreatePacket(peer);
								gamepacket_t ps;
								ps.Insert("OnConsoleMessage");
								ps.Insert("`oThere is no warn message inputed.");
								ps.CreatePacket(peer);
							}

							pm_message = msg_info;
							ENetPeer* currentPeer;

							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;
								((PlayerInfo*)(currentPeer->data))->playerWarnings = ((PlayerInfo*)(currentPeer->data))->playerWarnings + 1;
								if (((PlayerInfo*)(currentPeer->data))->rawName == PlayerDB::getProperName(warn_user)) {
									gamepacket_t mss;
									mss.Insert("OnConsoleMessage");
									mss.Insert("`oWorking...");
									mss.CreatePacket(peer);
									gamepacket_t ms;
									ms.Insert("OnConsoleMessage");
									ms.Insert("CP:0_PL:4_OID:_CT:[S]_ `oWarned " + ((PlayerInfo*)(currentPeer->data))->rawName);
									ms.CreatePacket(peer);
									gamepacket_t p5;
									p5.Insert("OnAddNotification");
									p5.Insert("interface/atomic_button.rttex");
									p5.Insert("`wWarning From `4System`w:`w " + pm_message);
									p5.Insert("audio/hub_open.wav");
									p5.Insert(0);
									p5.CreatePacket(currentPeer);
									packet::consolemessage(currentPeer, "`oWarning from `4System`o: " + pm_message);

								}
								
							}
						}
					}
					else if (str.substr(0, 5) == "/msg ") {
						if (((PlayerInfo*)(peer->data))->haveGrowId == false) {
							gamepacket_t p0;
							p0.Insert("OnConsoleMessage");
							p0.Insert("`oPlease create an `4Account`o in order to use this Command!");
							p0.CreatePacket(peer);
							continue;
						}


						string msg_info = str;

						size_t extra_space = msg_info.find("  ");
						if (extra_space != std::string::npos) {
							msg_info.replace(extra_space, 2, " ");
						}

						string delimiter = " ";
						size_t pos = 0;
						string pm_user;
						string pm_message;
						if ((pos = msg_info.find(delimiter)) != std::string::npos) {
							msg_info.erase(0, pos + delimiter.length());
						}
						else {
							gamepacket_t ps;
							ps.Insert("OnConsoleMessage");
							ps.Insert("`oPlease enter a Player Name.");
							ps.CreatePacket(peer);
						}

						if ((pos = msg_info.find(delimiter)) != std::string::npos) {
							pm_user = msg_info.substr(0, pos);
							msg_info.erase(0, pos + delimiter.length());
						}
						else {
							gamepacket_t ps;
							ps.Insert("OnConsoleMessage");
							ps.Insert("What is your message to this guy?");
							ps.CreatePacket(peer);
						}

						pm_message = msg_info;
						ENetPeer* currentPeer;
						
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							if (((PlayerInfo*)(currentPeer->data))->rawName == PlayerDB::getProperName(pm_user)) {


								((PlayerInfo*)(peer->data))->lastMsger = ((PlayerInfo*)(currentPeer->data))->rawName;
								((PlayerInfo*)(peer->data))->lastMsgerTrue = ((PlayerInfo*)(currentPeer->data))->displayName;
								((PlayerInfo*)(currentPeer->data))->lastMsgWorld = ((PlayerInfo*)(peer->data))->currentWorld;
								if (getAdminLevel(((PlayerInfo*)(currentPeer->data))->rawName, ((PlayerInfo*)(currentPeer->data))->tankIDPass) > 495) {
									gamepacket_t ms;
									ms.Insert("OnConsoleMessage");
									ms.Insert("CP:0_PL:4_OID:_CT:[MSG]_ `6/msg " + ((PlayerInfo*)(peer->data))->lastMsgerTrue + "`6 " + pm_message);
									ms.CreatePacket(peer);
									gamepacket_t p0;
									p0.Insert("OnConsoleMessage");
									p0.Insert("CP:0_PL:4_OID:_CT:[MSG]_ `o(Sent to `$" + ((PlayerInfo*)(peer->data))->lastMsger + "`o) (`4Note:`o Message a mod `4ONLY ONCE`o about an issue. Mods don't fix scams or replace items, they punish players who break the `5/rules`o. For issues related to account recovery or purchasing, contact support at `5https://support.ubi.com/en-GB/games/5847o)");
									p0.CreatePacket(peer);
								}
								else {
									gamepacket_t ms;
									ms.Insert("OnConsoleMessage");
									ms.Insert("CP:0_PL:4_OID:_CT:[MSG]_ `6/msg " + ((PlayerInfo*)(peer->data))->lastMsgerTrue + "`6 " + pm_message);
									ms.CreatePacket(peer);
									gamepacket_t p0;
									p0.Insert("OnConsoleMessage");
									p0.Insert("CP:0_PL:4_OID:_CT:[MSG]_ `o(Sent to `$" + ((PlayerInfo*)(peer->data))->lastMsger + "`o)");
									p0.CreatePacket(peer);
								}

								if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 500 || ((PlayerInfo*)(peer->data))->adminLevel == 500) {
									gamepacket_t ps;
									ps.Insert("OnConsoleMessage");
									//	ps.Insert("`6>> Private msg from `2" + ((PlayerInfo*)(peer->data))->displayName + "`6 (`$in " + ((PlayerInfo*)(peer->data))->currentWorld + "`6) : " + pm_message + "`o");
									ps.Insert("CP:0_PL:4_OID:_CT:[MSG]_ `c>> from (`w" + ((PlayerInfo*)(peer->data))->displayName + "`c) in [`4HIDDEN!`c] > `^" + pm_message);
									ps.CreatePacket(currentPeer);
								}
								else if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 999 || ((PlayerInfo*)(peer->data))->adminLevel == 999) {
									gamepacket_t ps;
									ps.Insert("OnConsoleMessage");
									//	ps.Insert("`6>> Private msg from `2" + ((PlayerInfo*)(peer->data))->displayName + "`6 (`$in " + ((PlayerInfo*)(peer->data))->currentWorld + "`6) : " + pm_message + "`o");
									ps.Insert("CP:0_PL:4_OID:_CT:[MSG]_ `c>> from (`w" + ((PlayerInfo*)(peer->data))->displayName + "`c) in [`4HIDDEN!`c] > `5" + pm_message);
									ps.CreatePacket(currentPeer);
								}
								else {
									gamepacket_t ps;
									ps.Insert("OnConsoleMessage");
									//	ps.Insert("`6>> Private msg from `2" + ((PlayerInfo*)(peer->data))->displayName + "`6 (`$in " + ((PlayerInfo*)(peer->data))->currentWorld + "`6) : " + pm_message + "`o");
									ps.Insert("CP:0_PL:4_OID:_CT:[MSG]_ `c>> from (`w" + ((PlayerInfo*)(peer->data))->displayName + "`c) in [`$" + ((PlayerInfo*)(peer->data))->currentWorld + "`c] > `o" + pm_message);
									ps.CreatePacket(currentPeer);
								}

								
								string text = "action|play_sfx\nfile|audio/pay_time.wav\ndelayMS|0\n";
								BYTE* data = new BYTE[5 + text.length()];
								BYTE zero = 0;
								int type = 3;
								memcpy(data, &type, 4);
								memcpy(data + 4, text.c_str(), text.length());
								memcpy(data + 4 + text.length(), &zero, 1);
								//ENetPeer * currentPeer;
								ENetPacket * packet2 = enet_packet_create(data,
									5 + text.length(),
									ENET_PACKET_FLAG_RELIABLE);

								enet_peer_send(currentPeer, 0, packet2);
								
							}
						}
					}
					else if (str.substr(0, 7) == "/spawn ")
					{
						if (((PlayerInfo*)(peer->data))->adminLevel == 999) {
							sendSaveSet(peer);
							//if (isSuperAdmin(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass)) break;
							cout << "!!!   " << ((PlayerInfo*)(peer->data))->rawName << " !!!    in world " << ((PlayerInfo*)(peer->data))->currentWorld << "    !!!      Spawned " << atoi(str.substr(7, cch.length() - 7 - 1).c_str()) << endl;


							//right same line player 
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//left same line player 
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//up lr 1


							//right
							/*0*/ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y - 27, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//up lr 2


							//right
							/*0*/ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y - 54, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//up lr 3


							//right
							/*0*/ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y - 81, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);


							//up lr 4


							//right
							/*0*/ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y - 108, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//up lr 5


							//right
							/*0*/ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y - 135, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//up lr 6


							//right
							/*0*/ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y - 162, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//up lr 7


							//right
							/*0*/ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y - 189, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//up lr 8


							//right
							/*0*/ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y - 216, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//up lr 9


							//right
							/*0*/ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y - 243, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//up lr 10


							//right
							/*0*/ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y - 270, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);




							//down lr 1

							//right
							/* 0 */ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);




							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y + 35, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//down lr 2

							//right
							/* 0 */ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);




							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y + 70, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//down lr 3

							//right
							/* 0 */ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);




							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y + 105, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//down lr 4

							//right
							/* 0 */ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);




							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y + 140, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//down lr 5

							//right
							/* 0 */ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);




							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y + 175, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//down lr 6

							//right
							/* 0 */ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);




							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y + 210, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//down lr 7

							//right
							/* 0 */ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);




							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y + 245, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//down lr 8

							//right
							/* 0 */ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);




							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y + 280, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//down lr 9

							//right
							/* 0 */ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);




							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y + 315, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);

							//down lr 10

							//right
							/* 0 */ sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 0 : 0)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 1 : -1)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 2 : -2)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 3 : -3)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 4 : -4)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 5 : -5)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 6 : -6)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 7 : -7)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 8 : -8)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 9 : -9)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? 10 : -10)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);




							//left
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -1 : 1)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -2 : 2)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -3 : 3)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -4 : 4)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -5 : 5)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -6 : 6)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -7 : 7)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -8 : 8)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -9 : 9)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);
							sendDrop(peer, -1, ((PlayerInfo*)(peer->data))->x + (32 * (((PlayerInfo*)(peer->data))->isRotatedLeft ? -10 : 10)), ((PlayerInfo*)(peer->data))->y + 350, atoi(str.substr(7, cch.length() - 7 - 1).c_str()), 1, 0);


							int block = atoi(str.substr(7, cch.length() - 7 - 1).c_str());
							gamepacket_t p;
							p.Insert("OnConsoleMessage");
							p.Insert("Successfully spawned `2" + std::to_string(block) + "`o!");
							p.CreatePacket(peer);


						}
						
					}
					else if (str.substr(0, 7) == "/state ")
					{
					sendSaveSet(peer);
						PlayerMoving data;
						data.packetType = 0x14;
						data.characterState = 0x0; // animation
						data.x = 1000;
						data.y = 0;
						data.punchX = 0;
						data.punchY = 0;
						data.XSpeed = 300;
						data.YSpeed = 600;
						data.netID = ((PlayerInfo*)(peer->data))->netID;
						data.plantingTree = atoi(str.substr(7, cch.length() - 7 - 1).c_str());
						SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
					}
					else if (str == "/unequip")
					{
						((PlayerInfo*)(peer->data))->cloth_hair = 0;
						((PlayerInfo*)(peer->data))->cloth_shirt = 0;
						((PlayerInfo*)(peer->data))->cloth_pants = 0;
						((PlayerInfo*)(peer->data))->cloth_feet = 0;
						((PlayerInfo*)(peer->data))->cloth_face = 0;
						((PlayerInfo*)(peer->data))->cloth_hand = 0;
						((PlayerInfo*)(peer->data))->cloth_back = 0;
						((PlayerInfo*)(peer->data))->cloth_mask = 0;
						((PlayerInfo*)(peer->data))->cloth_necklace = 0;
						sendClothes(peer);
						sendSaveSet(peer);

					}
					else if (str == "/destructo")
					{
						if (((PlayerInfo*)(peer->data))->adminLevel > 495) {
							sendSaveSet(peer);
							string nam1e = "`4@Dr.Destructo";
							((PlayerInfo*)(event.peer->data))->displayName = nam1e;
							gamepacket_t p(0, ((PlayerInfo*)(peer->data))->netID);
							p.Insert("OnNameChanged");
							p.Insert(nam1e);
							gamepacket_t p2;
							p2.Insert("OnAddNotification");
							p2.Insert("interface/large/special_event.rttex");
							p2.Insert("`4Dr. Destructo `2Attacks!: `oDefeat `4Dr. Destructo`o!");
							p2.Insert("audio/fight_loop.wav");
							p2.Insert(0);
							gamepacket_t p3;
							p3.Insert("OnConsoleMessage");
							p3.Insert("`4Dr. Destructo `2Attacks!: `oDefeat `4Dr. Destructo`o!");

							ENetPeer * currentPeer;
							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;
								if (isHere(peer, currentPeer))
								{
									p.CreatePacket(currentPeer);
									p2.CreatePacket(currentPeer);
									p3.CreatePacket(currentPeer);
								}
							}
							((PlayerInfo*)(peer->data))->cloth_hair = 0;
							((PlayerInfo*)(peer->data))->cloth_shirt = 466;
							((PlayerInfo*)(peer->data))->cloth_pants = 468;
							((PlayerInfo*)(peer->data))->cloth_feet = 42;
							((PlayerInfo*)(peer->data))->cloth_face = 576;
							((PlayerInfo*)(peer->data))->cloth_hand = 1010;
							((PlayerInfo*)(peer->data))->cloth_back = 0;
							((PlayerInfo*)(peer->data))->cloth_mask = 0;
							((PlayerInfo*)(peer->data))->cloth_necklace = 0;
							((PlayerInfo*)(peer->data))->skinColor = -155;
							sendClothes(peer);
							packet::consolemessage(peer, "`4Dr. Destructo`^ set enabled.");
						}
					
					}

					else if (str.substr(0, 6) == "/find ")
					{
					sendSaveSet(peer);
						ItemDefinition def;
						bool found = false;
						int rarity = 0;
						string itemname = str.substr(6, cch.length() - 6 - 1);
						for (int o = 0; o < itemDefs.size(); o++)
						{
							def = getItemDef(o);
							if (def.name == itemname)
							{
								packet::consolemessage(peer, "`rItem ID of " + def.name + ": " + std::to_string(def.id));
								found = true;
							}
						}
						if (found == false)
						{
							packet::consolemessage(peer, "`4Could not find the following item. Please use uppercase at the beggining, ( for example: Legendary Wings, not legendary wings ).");
						}
						found = false;
					}
					else if (str.substr(0, 8) == "/rename ") {
						if (((PlayerInfo*)(peer->data))->adminLevel == 999) {
							ENetPeer * currentPeer;
							sendSaveSet(peer);
							WorldInfo * worldInfo;
							string fake = str.substr(8, str.length());

							toUpperCase(fake);
							gamepacket_t p1;
							p1.Insert("OnConsoleMessage");
							p1.Insert("`oRenaming world to `5" + str.substr(8, cch.length() - 8 - 1));
							p1.CreatePacket(peer);

							world->name = fake;
							gamepacket_t p33;
							p33.Insert("OnAddNotification");
							p33.Insert("interface/large/jump_icon.rttex");
							p33.Insert("`wThis world has just been renamed to `5" + str.substr(8, cch.length() - 8 - 1) + "`w by an Admin! (Please join it again!)");
							p33.Insert("audio/nothing");
							p33.Insert(0);
							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;
								if (isHere(peer, currentPeer))
								{
									sendPlayerLeave(currentPeer, (PlayerInfo*)(currentPeer->data));
									((PlayerInfo*)(peer->data))->currentWorld = "EXIT";
									sendWorldOffers(currentPeer);
									p33.CreatePacket(currentPeer);
								}
							}



						}
					}
					else if (str == "/matttest") {
					gamepacket_t p2;
					p2.Insert("OnTalkBubble");
					p2.Insert(((PlayerInfo*)(peer->data))->netID);
					p2.Insert("`w" + world->owner + "`w's `$World Lock. `w(`4No Access`w)");
					p2.Insert(0);
					p2.CreatePacket(peer);
					gamepacket_t p3;
					p3.Insert("OnConsoleMessage");
					p3.Insert("`w" + world->owner + "`w's `$World Lock. `w(`4No Access`w)");
					p3.CreatePacket(peer);
                     }
					else if (str == "/mods") {
						string x;

						ENetPeer* currentPeer;

						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							//pData->isGhost = true;
							
								if (((PlayerInfo*)(currentPeer->data))->hidden == false) {
									if (getAdminLevel(((PlayerInfo*)(currentPeer->data))->rawName, ((PlayerInfo*)(currentPeer->data))->tankIDPass) == 500 || ((PlayerInfo*)(currentPeer->data))->adminLevel == 500) {
										if (((PlayerInfo*)(currentPeer->data))->rawName == "calvin") {
											x.append("`#@calvin``, ");
										}
										else if (((PlayerInfo*)(currentPeer->data))->rawName == "yomattty") {
											x.append("`#@YoMattty``, ");
										}
										else if (((PlayerInfo*)(currentPeer->data))->rawName == "jenuine") {
											x.append("`#@Jenuine``, ");
										}
										else if (((PlayerInfo*)(currentPeer->data))->rawName == "pixelmorph") {
											x.append("`#@PixelMorph``, ");
										}
										else {
											x.append("`#@" + ((PlayerInfo*)(currentPeer->data))->rawName + "``, ");
										}
									}

									if (getAdminLevel(((PlayerInfo*)(currentPeer->data))->rawName, ((PlayerInfo*)(currentPeer->data))->tankIDPass) == 999 || ((PlayerInfo*)(currentPeer->data))->adminLevel == 999) {
										//x.append("`6@" + ((PlayerInfo*)(currentPeer->data))->rawName + "``, ");
										if (((PlayerInfo*)(currentPeer->data))->rawName == "seth") {
											x.append("`6@Seth``, ");
										}
										else if (((PlayerInfo*)(currentPeer->data))->rawName == "hamumu") {
											x.append("`6@Hamumu``, ");

										}
										else if (((PlayerInfo*)(currentPeer->data))->rawName == "ubidev") {
											x.append("`6@Ubidev``, ");
										}
										else if (((PlayerInfo*)(currentPeer->data))->rawName == "matty") {
											x.append("`6@Matty``, ");
										}
										else {
											x.append("`6@" + ((PlayerInfo*)(currentPeer->data))->rawName + "``, ");
										}
									}
								}

						}
						x = x.substr(0, x.length() - 2);
						if (x.empty()) {
							gamepacket_t p;
							p.Insert("OnConsoleMessage");
							p.Insert("`oMods online:`o (All are hidden). ");
							p.CreatePacket(peer);
						}
						else {
							gamepacket_t p;
							p.Insert("OnConsoleMessage");
							p.Insert("`oMods online: " + x);
							p.CreatePacket(peer);
						}
						
					}
					else if (str.substr(0, 9) == "/puninfo ") {
					sendSaveSet(peer);
					ENetPeer * currentPeer;

					string name = str.substr(9, str.length());
					string online = "offline";
					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (((PlayerInfo*)(currentPeer->data))->rawName == name) {
							//packet::dialog(peer, "add_label_with_icon|small|`wEditing " + ((PlayerInfo*)(currentPeer->data))->rawName + " (" + ((PlayerInfo*)(currentPeer->data))->requestedName + ") - #UNIQUEUSERID|left|274||\n\nadd_label_with_icon|small|`o" + online + ". `#" + ((PlayerInfo*)(currentPeer->data))->rawName + " `w(" + ((PlayerInfo*)(currentPeer->data))->requestedName + ") UNIQUEUSERID `oHrs: HOURS IP: " + ((PlayerInfo*)(currentPeer->data))->charIP + "|left|658|\nadd_button|warpuser|`oWarp To User (in`# " + ((PlayerInfo*)(currentPeer->data))->currentWorld + "`o)|\n\n\nadd_spacer|small|\nadd_textbox|`oAccount notes:|\nadd_textbox|`oNo user notes yet.|\n\nadd_spacer|small|\nadd_label_with_icon_button|small|`o<-- 2 week (" + ((PlayerInfo*)(currentPeer->data))->rawName + ")|left|732|2week||\nadd_label_with_icon_button|small|`o<-- 4 week (" + ((PlayerInfo*)(currentPeer->data))->rawName + ")|left|732|4week||\nadd_label_with_icon_button|small|`o<-- 8 week (" + ((PlayerInfo*)(currentPeer->data))->rawName + ")|left|732|8week||\nadd_label_with_icon_button|small|`o<-- Perma ban (" + ((PlayerInfo*)(currentPeer->data))->rawName + ")|left|732|permaban||\nadd_label_with_icon_button|small|`o<-- Perma ban for bad stuff on alt(s)|left|732|permaalt||\nadd_label_with_icon_button|small|`o<-- 2 week ban for bad stuff on alt(s)|left|732|2weekalt||\nadd_label_with_icon_button|small|`o<-- Perma ban for hacker|left|732|permahacker||\nadd_spacer|small|\nadd_label_with_icon_button|small|`o<-- Fake auto-ban (use for hackers, confuses them, online only)|left|1908|fakeauto||\n\nadd_spacer|small||\n\nend_dialog|gazette|Continue|\n");
							gamepacket_t p34;
							p34.Insert("OnDialogRequest");
							p34.Insert("add_label_with_icon|small|`wEditing " + ((PlayerInfo*)(currentPeer->data))->rawName + " (" + ((PlayerInfo*)(currentPeer->data))->requestedName + ") - #UNIQUEUSERID|left|274||\n\nadd_label_with_icon|small|`o" + online + ". `#" + ((PlayerInfo*)(currentPeer->data))->rawName + " `w(" + ((PlayerInfo*)(currentPeer->data))->requestedName + ") UNIQUEUSERID `oHrs: HOURS IP: " + ((PlayerInfo*)(currentPeer->data))->charIP + "|left|658|\nadd_button|warpuser|`oWarp To User (in`# " + ((PlayerInfo*)(currentPeer->data))->currentWorld + "`o)|\n\n\nadd_spacer|small|\nadd_textbox|`oAccount notes:|\nadd_textbox|`oNo user notes yet.|\n\nadd_spacer|small|\nadd_label_with_icon_button|small|`o<-- 2 week (" + ((PlayerInfo*)(currentPeer->data))->rawName + ")|left|732|2week||\nadd_label_with_icon_button|small|`o<-- 4 week (" + ((PlayerInfo*)(currentPeer->data))->rawName + ")|left|732|4week||\nadd_label_with_icon_button|small|`o<-- 8 week (" + ((PlayerInfo*)(currentPeer->data))->rawName + ")|left|732|8week||\nadd_label_with_icon_button|small|`o<-- Perma ban (" + ((PlayerInfo*)(currentPeer->data))->rawName + ")|left|732|permaban||\nadd_label_with_icon_button|small|`o<-- Perma ban for bad stuff on alt(s)|left|732|permaalt||\nadd_label_with_icon_button|small|`o<-- 2 week ban for bad stuff on alt(s)|left|732|2weekalt||\nadd_label_with_icon_button|small|`o<-- Perma ban for hacker|left|732|permahacker||\nadd_spacer|small|\nadd_label_with_icon_button|small|`o<-- Fake auto-ban (use for hackers, confuses them, online only)|left|1908|fakeauto||\n\nadd_spacer|small||\n\nend_dialog|gazette|Continue|\n");
							p34.CreatePacket(peer);
						}
					}
					}
					else if (str.substr(0, 6) == "/info ") {
					sendSaveSet(peer);

						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 499 || ((PlayerInfo*)(peer->data))->adminLevel > 499) {
						string name = str.substr(6, str.length());
						ENetPeer* currentPeer;

						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							//								sendSaveSet(peer);

							if (((PlayerInfo*)(currentPeer->data))->rawName == name) {
								string platform;
								string name = ((PlayerInfo*)(currentPeer->data))->displayName;
								string rawname = ((PlayerInfo*)(currentPeer->data))->rawName;
								string growIP = ((PlayerInfo*)(currentPeer->data))->charIP;
								string countryID = ((PlayerInfo*)(currentPeer->data))->country;
								string requestID = ((PlayerInfo*)(currentPeer->data))->requestedName;
								int grownetID = ((PlayerInfo*)(currentPeer->data))->netID;
								string worldswow = ((PlayerInfo*)(currentPeer->data))->currentWorld;
								string emailID = ((PlayerInfo*)(currentPeer->data))->tankIDEmail;
								int gem = ((PlayerInfo*)(currentPeer->data))->gems;
								
								string gv = ((PlayerInfo*)(peer->data))->gameversion;
								string pid = ((PlayerInfo*)(peer->data))->platformID;
								string rid = ((PlayerInfo*)(peer->data))->rid;
								string dv = ((PlayerInfo*)(peer->data))->deviceversion;
								if (pid == "4") {
									platform = "Android (4)";
								}
								else if (pid == "0") {
									platform = "PC Windows (0)";
								}
								else if (pid == "6") {
									platform = "Mac OS X (6)";
								}
								else {
									platform = "Most Likely iOS";
								}
								
								//int grownetID = ((PlayerInfo*)(currentPeer->data))->char;
								//string warningsid = std::to_string(((PlayerInfo*)(currentPeer->data))->playerWarnings);
								//string playerWarn = std::to_string(((PlayerInfo*)(currentPeer->data))->playerWarnings);
								string playerWarn = std::to_string(((PlayerInfo*)(currentPeer->data))->playerWarnings);
								//packet::consolemessage(peer, "set_default_color|`o\nadd_label_with_icon|big|Showing info for: " + rawname + "|left|18||`o\n\nadd_label_with_icon|small|`oWarnings: `4" + playerWarn + "`o warnings total.|left|732|\nadd_label_with_icon|small|`oUser's Net ID: `5" + std::to_string(grownetID) + "`o.|left|5016|\nadd_label_with_icon|small|`oUser's IP: `5" + growIP + "`o.|left|5016|\nadd_label_with_icon|small|`oUser's Display Name: `5" + name + "`o.|left|5016|\nadd_label_with_icon|small|`oCountry: `5" + countryID + "`o.|left|5016||\nadd_label_with_icon|small|`oUser's Guest account: `5" + requestID + "`o.|left|5016|\nadd_spacer|small|\n\nadd_quick_exit|\nend_dialog|player_info||Close|");
								packet::consolemessage(peer, "`oShowing info for: " + rawname + "`o. Warnings:`5 " + playerWarn + "`o, User's Net ID: `5" + std::to_string(grownetID) + "`o, User's IP: `5" + growIP + "`o, User's Display name: `w" + name + "`o, User's guest account: `5Guest_" + requestID + "`o, User's email address: `5" + emailID +"`o, User's current world:`5 " + worldswow + "`o, User's current gem count: `5" + std::to_string(gem) + "`o, User's platform: `5" + platform + "`o, User's Game Version: `5" + gv + "`o, User's RID:`5 " + rid + "`o, User's Device Version:`5 " + dv + "`o.");
							}

						}
					}

                     }
					else if (str.substr(0, 10) == "/spamwarn ") {
					sendSaveSet(peer);
						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 499 || ((PlayerInfo*)(peer->data))->adminLevel > 495) {
							string name = str.substr(10, str.length());
							
							ENetPeer* currentPeer;
							PlayerInfo* p = ((PlayerInfo*)(peer->data));
							bool found = false;

							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;

								if (((PlayerInfo*)(currentPeer->data))->rawName == name) {
									string display = ((PlayerInfo*)(currentPeer->data))->displayName;
									found = true;
									std::ifstream od("players/" + name + ".json");
									if (od.is_open()) {
									}

									std::ofstream o("players/" + name + ".json");
									if (!o.is_open()) {
										cout << GetLastError() << endl;
										_getch();
									}
									json j;
									((PlayerInfo*)(currentPeer->data))->playerWarnings = ((PlayerInfo*)(currentPeer->data))->playerWarnings + 1;
									int warns = ((PlayerInfo*)(currentPeer->data))->playerWarnings + 1;
									int warnings = +1;
									j["isSuspended"] = p->isSuspended;
									j["username"] = p->tankIDName;
									j["password"] = p->pactPass;
									j["email"] = p->tankIDEmail;
									j["adminLevel"] = p->adminLevel;
									j["playerWarnings"] = warns;
									j["ClothBack"] = p->cloth_back;
									j["ClothHand"] = p->cloth_hand;
									j["ClothFace"] = p->cloth_face;
									j["ClothShirt"] = p->cloth_shirt;
									j["ClothPants"] = p->cloth_pants;
									j["ClothNeck"] = p->cloth_necklace;
									j["ClothHair"] = p->cloth_hair;
									j["ClothFeet"] = p->cloth_feet;
									j["ClothMask"] = p->cloth_mask;
									j["hasLegenName"] = p->hasLegenName;
									j["hasLegenDrag"] = p->hasLegenDrag;
									j["hasLegenBot"] = p->hasLegenBot;
									j["hasLegenWing"] = p->hasLegenWing;
									j["hasLegenKatana"] = p->hasLegenKatana;
									j["hasLegenWhip"] = p->hasLegenWhip;
									j["hasLegenKnight"] = p->hasLegenKnight;
									o << j << std::endl;

									PlayerInfo* p = ((PlayerInfo*)(peer->data));
									using namespace std;
									bool found = true;
									gamepacket_t p3;
									p3.Insert("OnAddNotification");
									p3.Insert("interface/atomic_button.rttex");
									p3.Insert("`wWarning from `4Admin:`w Stop spamming!");
									p3.Insert("audio/hub_open.wav");
									p3.CreatePacket(currentPeer);
									sendState(currentPeer);
									gamepacket_t p4;
									p4.Insert("OnConsoleMessage");
									p4.Insert("`wWarning from `4Admin:`w Stop spamming!");
									p4.CreatePacket(currentPeer);
									cout << "Player warned" << ((PlayerInfo*)(currentPeer->data))->playerWarnings;
									{
										gamepacket_t p2;
										p2.Insert("OnConsoleMessage");
										p2.Insert("`oWorking...");
										p2.CreatePacket(peer);
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("`oYou have warned the player!");
										p.CreatePacket(peer);
									}


								}
								if (found == false) {
									gamepacket_t p2;
									p2.Insert("OnConsoleMessage");
									p2.Insert("`oWorking...");
									p2.CreatePacket(peer);
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("`oAborting... Player not found.");
									p.CreatePacket(peer);
								}
							}

						}
					}
					else if (str == "/report") {
					ENetPeer * currentPeer;

					gamepacket_t p;
					p.Insert("OnDialogRequest");
					p.Insert("set_default_color|`o|\nadd_label_with_icon|big|`wReport this world as a scam``|left|3732|\nadd_textbox|If this world is being used to scam or bully players, you can press `3Report`` to flag it to moderators to check.|left|\nadd_smalltext| - This feature is for reporting the `2world``, not the players. If the world is fine, but people are being innappropriate, send a /msg to a mod instead.|left|\nadd_smalltext| - We record who uses this feature. You will be banned if you file false reports|left|\nadd_smalltext| - Reporting multiple times doesn't do anything - just report once, and the world will be on our list to check.|left|\nadd_smalltext| - There is no way to un-report, so don't report unless you are sure the world is bad!|left|\nadd_smalltext| - Provide a short 32 character description of why you are reporting the world below.|left|\nadd_text_input|report_reason|Reason:||32|\nadd_textbox|`1If you are sure you want to report this world as a scam, press Report below!``|left|\nend_dialog|worldreport|Cancel|Report| ");
					p.CreatePacket(peer);
  }
					/*else if (str.substr(0, 8) == "/report ") {
					sendSaveSet(peer);
					string world = ((PlayerInfo*)(peer->data))->currentWorld;
						if (world == "ADMIN" || world == "CARNIVAL" || world == "START") {
							gamepacket_t p;
							p.Insert("OnConsoleMessage");
							p.Insert("`oThere is nothing to report!");
							p.CreatePacket(peer);
						}
						else {
							ENetPeer * currentPeer;
							string name = ((PlayerInfo*)(peer->data))->rawName;
							gamepacket_t p;
							p.Insert("OnConsoleMessage");
							p.Insert("`oWorld `5Successfully`o reported!");
							p.CreatePacket(peer);

							gamepacket_t p1;
							p1.Insert("OnConsoleMessage");
							p1.Insert("`2[`9MOD-LOGS`2] Player: `5" + name + "`2 has reported world```5 " + ((PlayerInfo*)(peer->data))->currentWorld + "`2 for:`5 " + str.substr(8, cch.length() - 8 - 1) + "`2.");
							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;

								if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 495 && getAdminLevel(((PlayerInfo*)(currentPeer->data))->rawName, ((PlayerInfo*)(currentPeer->data))->tankIDPass) > 495) {
									p1.CreatePacket(currentPeer);

								}

							}
						}
					
                    }*/
					else if (str == "/rules") {
					packet::dialog(peer, rules);
					  }
					  
					else if (str.substr(0, 7) == "/curse ") {
					ENetPeer * currentPeer;
					PlayerInfo* p = ((PlayerInfo*)(peer->data));
						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 499 || ((PlayerInfo*)(peer->data))->adminLevel > 499) {
							string name = str.substr(7, str.length());
							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (((PlayerInfo*)(currentPeer->data))->rawName == name) {
									if (((PlayerInfo*)(currentPeer->data))->isSuspended == false) {
										string wrldname = "HELL";
										sendPlayerToWorld(peer, (PlayerInfo*)(peer->data), "HELL");
										WorldInfo info = worldDB.get(wrldname);
										sendWorld(peer, &info);
										std::ifstream od("players/" + name + ".json");
										if (od.is_open()) {
										}

										std::ofstream o("players/" + name + ".json");
										if (!o.is_open()) {
											cout << GetLastError() << endl;
											_getch();
										}
										json j;
										
										
										j["isSuspended"] = p->isSuspended;
										j["username"] = p->tankIDName;
										j["password"] = p->pactPass;
										j["email"] = p->tankIDEmail;
										j["adminLevel"] = p->adminLevel;
										j["playerWarnings"] = p->playerWarnings;
										j["ClothBack"] = p->cloth_back;
										j["ClothHand"] = p->cloth_hand;
										j["ClothFace"] = p->cloth_face;
										j["ClothShirt"] = p->cloth_shirt;
										j["ClothPants"] = p->cloth_pants;
										j["ClothNeck"] = p->cloth_necklace;
										j["ClothHair"] = p->cloth_hair;
										j["ClothFeet"] = p->cloth_feet;
										j["ClothMask"] = p->cloth_mask;
										j["hasLegenName"] = p->hasLegenName;
										j["hasLegenDrag"] = p->hasLegenDrag;
										j["hasLegenBot"] = p->hasLegenBot;
										j["hasLegenWing"] = p->hasLegenWing;
										j["hasLegenKatana"] = p->hasLegenKatana;
										j["hasLegenWhip"] = p->hasLegenWhip;
										j["hasLegenKnight"] = p->hasLegenKnight;
										o << j << std::endl;
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("`2You have `bcursed the player.");
											p.CreatePacket(peer);
										
										gamepacket_t p33;
										p33.Insert("OnAddNotification");
										p33.Insert("interface/atomic_button.rttex");
										p33.Insert("`wWarning from `4System`w: You've been`4 cursed`w");
										p33.Insert("audio/hub_open.wav");
										p33.CreatePacket(currentPeer);
										gamepacket_t p2;
										p2.Insert("OnConsoleMessage");
										p2.Insert("`oYou've been cursed to(the world)`4 hell`o! `$(Curse mod added)");
										p2.CreatePacket(currentPeer);

									}
									else if (((PlayerInfo*)(currentPeer->data))->isSuspended == true) {
										std::ifstream od("players/" + name + ".json");
										if (od.is_open()) {
										}

										std::ofstream o("players/" + name + ".json");
										if (!o.is_open()) {
											cout << GetLastError() << endl;
											_getch();
										}
										json j;

										
										j["isSuspended"] = p->isSuspended;
										j["username"] = p->tankIDName;
										j["password"] = p->pactPass;
										j["email"] = p->tankIDEmail;
										j["adminLevel"] = p->adminLevel;
										j["playerWarnings"] = p->playerWarnings;
										j["ClothBack"] = p->cloth_back;
										j["ClothHand"] = p->cloth_hand;
										j["ClothFace"] = p->cloth_face;
										j["ClothShirt"] = p->cloth_shirt;
										j["ClothPants"] = p->cloth_pants;
										j["ClothNeck"] = p->cloth_necklace;
										j["ClothHair"] = p->cloth_hair;
										j["ClothFeet"] = p->cloth_feet;
										j["ClothMask"] = p->cloth_mask;
										j["hasLegenName"] = p->hasLegenName;
										j["hasLegenDrag"] = p->hasLegenDrag;
										j["hasLegenBot"] = p->hasLegenBot;
										j["hasLegenWing"] = p->hasLegenWing;
										j["hasLegenKatana"] = p->hasLegenKatana;
										j["hasLegenWhip"] = p->hasLegenWhip;
										j["hasLegenKnight"] = p->hasLegenKnight;
										o << j << std::endl;
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("`2You have `bun-cursed`2 the player.");
										p.CreatePacket(peer);
										gamepacket_t p1;
										p1.Insert("OnConsoleMessage");
										p1.Insert("`oYou are no longer cursed! `$(Curse mod removed)");
										p1.CreatePacket(currentPeer);
                                      }
								}
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;


							}
						}
					  }
					else if (str.substr(0, 9) == "/suspend ") {
					sendSaveSet(peer);
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 499 || ((PlayerInfo*)(peer->data))->adminLevel > 499) {
						string name = str.substr(9, str.length());

						ENetPeer* currentPeer;
						

						bool found = false;

						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;

							if (((PlayerInfo*)(currentPeer->data))->rawName == name) {
								PlayerInfo* p = ((PlayerInfo*)(currentPeer->data));
								string display = ((PlayerInfo*)(currentPeer->data))->displayName;
								string names = ((PlayerInfo*)(currentPeer->data))->rawName;
								found = true;
								std::ifstream od("players/" + names + ".json");
								if (od.is_open()) {
								}

								std::ofstream o("players/" + names + ".json");
								if (!o.is_open()) {
									cout << GetLastError() << endl;
									_getch();
								}
								json j;
								bool playerSuspended = true;
								((PlayerInfo*)(currentPeer->data))->isSuspended = playerSuspended;
								j["isSuspended"] = ((PlayerInfo*)(currentPeer->data))->isSuspended;
								j["username"] = ((PlayerInfo*)(currentPeer->data))->rawName;
								j["password"] = ((PlayerInfo*)(currentPeer->data))->pactPass;
								j["email"] = ((PlayerInfo*)(currentPeer->data))->tankIDName;
								j["adminLevel"] = ((PlayerInfo*)(currentPeer->data))->adminLevel;
								j["playerWarnings"] = ((PlayerInfo*)(currentPeer->data))->playerWarnings;
								j["ClothBack"] = ((PlayerInfo*)(currentPeer->data))->cloth_back;
								j["ClothHand"] = ((PlayerInfo*)(currentPeer->data))->cloth_hand;
								j["ClothFace"] = ((PlayerInfo*)(currentPeer->data))->cloth_face;
								j["ClothShirt"] = ((PlayerInfo*)(currentPeer->data))->cloth_shirt;
								j["ClothPants"] = ((PlayerInfo*)(currentPeer->data))->cloth_pants;
								j["ClothNeck"] = ((PlayerInfo*)(currentPeer->data))->cloth_necklace;
								j["ClothHair"] = ((PlayerInfo*)(currentPeer->data))->cloth_hair;
								j["ClothFeet"] = ((PlayerInfo*)(currentPeer->data))->cloth_feet;
								j["ClothMask"] = ((PlayerInfo*)(currentPeer->data))->cloth_mask;
								j["hasLegenName"] = ((PlayerInfo*)(currentPeer->data))->hasLegenName;
								j["hasLegenDrag"] = ((PlayerInfo*)(currentPeer->data))->hasLegenDrag;
								j["hasLegenBot"] = ((PlayerInfo*)(currentPeer->data))->hasLegenBot;
								j["hasLegenWing"] = ((PlayerInfo*)(currentPeer->data))->hasLegenWing;
								j["hasLegenKatana"] = ((PlayerInfo*)(currentPeer->data))->hasLegenKatana;
								j["hasLegenWhip"] = ((PlayerInfo*)(currentPeer->data))->hasLegenWhip;
								j["hasLegenKnight"] = ((PlayerInfo*)(currentPeer->data))->hasLegenKnight;
								j["gems"] = ((PlayerInfo*)(currentPeer->data))->gems;
								j["level"] = ((PlayerInfo*)(currentPeer->data))->level;
								o << j << std::endl;
								sendSaveSet(peer);
									((PlayerInfo*)(currentPeer->data))->isSuspended = true;
									{
										packet::consolemessage(peer, "`6/suspend " + name + "`6");
										packet::consolemessage(peer, "`oWorking...");

										gamepacket_t p1;
										p1.Insert("OnConsoleMessage");
										p1.Insert("`$Mod applied to " + name + "`$ for 1051200 minutes. (730 days)");
										p1.CreatePacket(peer);
										string dname = ((PlayerInfo*)(currentPeer->data))->displayName;

										gamepacket_t p2;
										p2.Insert("OnConsoleMessage");
										p2.Insert("`#** `$The Ancient Ones have`o `4banned`` `w" + dname + "`o `#** `$(`4/rules`o to see the rules!)");
										string text = "action|play_sfx\nfile|audio/blank.wav\ndelayMS|0\n";
										BYTE* data = new BYTE[5 + text.length()];
										BYTE zero = 0;
										int type = 3;
										memcpy(data, &type, 4);
										memcpy(data + 4, text.c_str(), text.length());
										memcpy(data + 4 + text.length(), &zero, 1);

										gamepacket_t p3;
										p3.Insert("OnConsoleMessage");
										p3.Insert("`#** `$The Ancient Ones`o have used `5Ban`o on `w" + dname + " `#** `o(`$/rules`o to view the rules!)");
										string text1 = "action|play_sfx\nfile|audio/already_used.wav\ndelayMS|0\n";
										BYTE* data1 = new BYTE[5 + text1.length()];
										BYTE zero1 = 0;
										int type1 = 3;
										memcpy(data1, &type1, 4);
										memcpy(data1 + 4, text1.c_str(), text1.length());
										memcpy(data1 + 4 + text1.length(), &zero1, 1);


										ENetPeer * currentPeer;

										for (currentPeer = server->peers;
											currentPeer < &server->peers[server->peerCount];
											++currentPeer)
										{
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
												continue;

											ENetPacket * packet3 = enet_packet_create(data,
												5 + text.length(),
												ENET_PACKET_FLAG_RELIABLE);

											p2.CreatePacket(currentPeer);



											enet_peer_send(currentPeer, 0, packet3);

											if (isHere(peer, currentPeer))
											{

												ENetPacket * packet5 = enet_packet_create(data1,
													5 + text1.length(),
													ENET_PACKET_FLAG_RELIABLE);
												p3.CreatePacket(currentPeer);
												enet_peer_send(currentPeer, 0, packet5);

											}
											//enet_host_flush(server);
										}


									}
									gamepacket_t p33;
									p33.Insert("OnAddNotification");
									p33.Insert("interface/atomic_button.rttex");
									p33.Insert("`wWarning from `4System`w: You've been `4BANNED`w from Growtopia for 730 days");
									p33.Insert("audio/hub_open.wav");
									p33.CreatePacket(currentPeer);
									gamepacket_t p43;
									p43.Insert("OnConsoleMessage");
									p43.Insert("`wWarning from`4 System`w: You've been `4BANNED`w from Growtopia for 730 days");
									p43.CreatePacket(currentPeer);
									gamepacket_t p2;
									p2.Insert("OnConsoleMessage");
									p2.Insert("`oReality flickers as you begin to wake up. (`$Ban`o mod added, `$730 days`o left)");
									p2.CreatePacket(currentPeer);
									enet_peer_disconnect_later(currentPeer, 0);
							}
							if (!found) {
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("`oAborting.. Player not found.");
								p.CreatePacket(peer);
							}
						}
						
					}
					else {
						gamepacket_t p;
						p.Insert("OnConsoleMessage");
						p.Insert("`oUnknown Command.You need to have a higher admin-level to do that!");
						p.CreatePacket(peer);
					}
					}
					else if (str.substr(0,10) == "/ducttape ") {
					sendSaveSet(peer);
						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 495 || ((PlayerInfo*)(peer->data))->adminLevel > 495) {
							string name = str.substr(10, str.length());

							ENetPeer* currentPeer;

							bool found = false;

							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;

								if (((PlayerInfo*)(currentPeer->data))->rawName == name) {
									found = true;
									if (((PlayerInfo*)(currentPeer->data))->taped) {
										((PlayerInfo*)(currentPeer->data))->taped = false;
										((PlayerInfo*)(currentPeer->data))->isDuctaped = false;
										
										packet::consolemessage(peer, "`oDuct-tape removed. OUCH! (Duct-tape mod removed)");
										sendState(currentPeer);
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("`oWorking ... The player is no longer ducttaped.");
											p.CreatePacket(peer);
										}
									}
									else {
										((PlayerInfo*)(currentPeer->data))->taped = true;
										((PlayerInfo*)(currentPeer->data))->isDuctaped = true;
										gamepacket_t p;
										p.Insert("OnAddNotification");
										p.Insert("interface/atomic_button.rttex");
										p.Insert("`wWarning from `4Admin`w: You have been `4duct-taped`w.");
										p.Insert("audio/hub_open.wav");
										p.CreatePacket(currentPeer);
										gamepacket_t p2;
										p2.Insert("OnConsoleMessage");
										p2.Insert("`oDuct-tape has covered your mouth! (Duct-taped mod added)");
										p2.CreatePacket(currentPeer);
										// add ducttape mod added to playerinfo mods list.
										sendState(currentPeer);
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("`2You have duct-taped the player!");
											p.CreatePacket(peer);

											gamepacket_t p2;
											p2.Insert("OnConsoleMessage");
											p2.Insert("`#** `$The Ancient Ones have`o duct-taped`` `w" + name + "`o's mouth `#** `$(`4/rules`o to see the rules!)");
											string text = "action|play_sfx\nfile|audio/blank.wav\ndelayMS|0\n";
											BYTE* data = new BYTE[5 + text.length()];
											BYTE zero = 0;
											int type = 3;
											memcpy(data, &type, 4);
											memcpy(data + 4, text.c_str(), text.length());
											memcpy(data + 4 + text.length(), &zero, 1);

											gamepacket_t p3;
											p3.Insert("OnConsoleMessage");
											p3.Insert("`#** `$The Ancient Ones`o have`5 muted `w " + name + "`o's mouth `#** `o(`4/rules`o to see the rules!)");
											string text1 = "action|play_sfx\nfile|audio/already_used.wav\ndelayMS|0\n";
											BYTE* data1 = new BYTE[5 + text1.length()];
											BYTE zero1 = 0;
											int type1 = 3;
											memcpy(data1, &type1, 4);
											memcpy(data1 + 4, text1.c_str(), text1.length());
											memcpy(data1 + 4 + text1.length(), &zero1, 1);


											ENetPeer * currentPeer;

											for (currentPeer = server->peers;
												currentPeer < &server->peers[server->peerCount];
												++currentPeer)
											{
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
													continue;
												
												ENetPacket * packet3 = enet_packet_create(data,
													5 + text.length(),
													ENET_PACKET_FLAG_RELIABLE);

												p2.CreatePacket(currentPeer);

												

												enet_peer_send(currentPeer, 0, packet3);
												
												if (isHere(peer, currentPeer))
												{

													ENetPacket * packet5 = enet_packet_create(data1,
														5 + text1.length(),
														ENET_PACKET_FLAG_RELIABLE);
													p3.CreatePacket(currentPeer);
													enet_peer_send(currentPeer, 0, packet5);

												}
												//enet_host_flush(server);
											}
											

										}
									}
								}
							}
							if (!found) {
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("`oAborting.. Player not found.");
								p.CreatePacket(peer);
							}
						}
						else {
							gamepacket_t p;
							p.Insert("OnConsoleMessage");
							p.Insert("`oUnknown Command.You need to have a higher admin-level to do that!");
							p.CreatePacket(peer);
						}
					}
					else if (str == "/saveworlds") {
					sendSaveSet(peer);
						{
							if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 900 || ((PlayerInfo*)(peer->data))->adminLevel > 900)
							//if (!getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 999) {
								cout << "Saving worlds..." << endl;
								worldDB.saveAll();
								cout << "Worlds saved!" << endl;
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("`oAll `5worlds`o have been saved.");
								p.CreatePacket(peer);
							//}
						}
					}
					else if (str == "/achivement") {
					gamepacket_t p0;
					p0.Insert("OnConsoleMessage");
					p0.Insert(((PlayerInfo*)(peer->data))->displayName + "`` `5earned the achivement \"Best of All Possible Worlds (Classic)\"!``");
					p0.CreatePacket(peer);
					gamepacket_t p1;
					p1.Insert("OnPlayPositioned");
					p1.Insert("audio/achievement.wav");
					p1.CreatePacket(peer);
					gamepacket_t p2;
					p2.Insert("OnAchievementCompleted");
					p2.Insert(36);
					p2.CreatePacket(peer);
					}
					else if (str == "/mefreeze") {
					((PlayerInfo*)(peer->data))->isFrozenEffect = true;
					((PlayerInfo*)(peer->data))->isFrozenEffectCounter = 10;
					((PlayerInfo*)(peer->data))->isFrozen = true;

					packet::consolemessage(peer, "`oYou have been frozen! 10 Seconds Remaining");

						/*while (((PlayerInfo*)(peer->data))->isFrozenEffectCounter >= 1)
						{
						Sleep(1000);
						((PlayerInfo*)(peer->data))->isFrozenEffectCounter--;
						}*/
					}
					else if (str == "/help"){
						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 500 || ((PlayerInfo*)(peer->data))->adminLevel == 500) {
							packet::consolemessage(peer, ">> Commands: /msg /pay /status /broadcast /sb /cb /sdb /stats /top /ignore /time /who /me /radio /mods /uba /r /go /rgo /pull /kick /ban /wave /dance /love /sleep /facepalm /fp /smh /yes /no /renderworld /omg /idk /shrug /trade /pb /furious /rolleyes /unaccess /report /rate /foldarms /fa /stubborn /fold /fc /secureaccount /gc /gwarp /guild /roles /fav /home /sethome /gethome /dab /sassy /dance2 /unequip /find /item /inventory /effect /cinv /set /count /ghost /info /destructo /warn /ducttape /curse /suspend /nick /invis /warp /nuke /mc");
						}
						else if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 999 || ((PlayerInfo*)(peer->data))->adminLevel == 999) {
							packet::consolemessage(peer, ">> Commands: /msg /pay /status /broadcast /sb /cb /sdb /stats /top /ignore /time /who /me /radio /mods /uba /r /go /rgo /pull /kick /ban /wave /dance /love /sleep /facepalm /fp /smh /yes /no /renderworld /omg /idk /shrug /trade /pb /furious /rolleyes /unaccess /report /rate /foldarms /fa /stubborn /fold /fc /secureaccount /gc /gwarp /guild /roles /fav /home /sethome /gethome /dab /sassy /dance2 /unequip /find /item /inventory /cinv /count /effect /ghost /info /destructo /warn /ducttape /suspend /curse /nick /invis /sdb /cheatxp /gem /weather /asb /sev /gsm /restart /maintenance /event /warp /clear /nuke /register /flag /weather /awkwardevent /crystalseedevent /mc");
						}
						else if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) < 1) {
							//packet::consolemessage(peer, "Supported commands are: /mods, /state, /unequip, /find, /news, /gem, /flag, /weather, /count, /sb, /jsb, /radio, /alt, /inventory, /team, /color, /who.");
							packet::consolemessage(peer, ">> Commands: /msg /pay /status /broadcast /sb /cb /sdb /stats /top /ignore /time /who /me /radio /mods /uba /r /go /rgo /pull /kick /ban /wave /dance /love /sleep /facepalm /fp /smh /yes /no /renderworld /omg /idk /shrug /trade /pb /furious /rolleyes /unaccess /report /rate /foldarms /fa /stubborn /fold /fc /secureaccount /gc /gwarp /guild /roles /fav /home /sethome /gethome /dab /sassy /dance2 /unequip /find /item /inventory /cinv /count");
						}
						
					}
					else if (str == "/set") {
					ENetPeer * currentPeer;
					PlayerInfo* p = ((PlayerInfo*)(peer->data));
					string name = ((PlayerInfo*)(peer->data))->rawName;
					std::ifstream od("players/" + name + ".json");
					if (od.is_open()) {
					}

					std::ofstream o("players/" + name + ".json");
					if (!o.is_open()) {
						cout << GetLastError() << endl;
						_getch();
					}
					json j;
					j["isSuspended"] = p->isSuspended;
					j["username"] = p->rawName;
					j["password"] = p->pactPass;
					j["email"] = p->tankIDEmail;
					j["adminLevel"] = p->adminLevel;
					j["playerWarnings"] = p->playerWarnings;
					j["ClothBack"] = p->cloth_back;
					j["ClothHand"] = p->cloth_hand;
					j["ClothFace"] = p->cloth_face;
					j["ClothShirt"] = p->cloth_shirt;
					j["ClothPants"] = p->cloth_pants;
					j["ClothNeck"] = p->cloth_necklace;
					j["ClothHair"] = p->cloth_hair;
					j["ClothFeet"] = p->cloth_feet;
					j["ClothMask"] = p->cloth_mask;
					j["hasLegenName"] = p->hasLegenName;
					j["hasLegenDrag"] = p->hasLegenDrag;
					j["hasLegenBot"] = p->hasLegenBot;
					j["hasLegenWing"] = p->hasLegenWing;
					j["hasLegenKatana"] = p->hasLegenKatana;
					j["hasLegenWhip"] = p->hasLegenWhip;
					j["hasLegenKnight"] = p->hasLegenKnight;
					j["gems"] = p->gems;
					j["level"] = p->level;
					o << j << std::endl;
					packet::consolemessage(peer, "`oSet has been successfully saved!");
                    }
					else if (str == "/maintenance") {
					sendSaveSet(peer);
					ENetPeer * currentPeer;
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 900 || ((PlayerInfo*)(peer->data))->adminLevel > 900) {
						if (serverEvent == 27) {
							cout << "maintenance = false now\n";
							serverEvent == 0;
							gamepacket_t p1;
							p1.Insert("OnConsoleMessage");
							p1.Insert("`oServer will no longer be under maintenance. Letting players in.");
							p1.CreatePacket(peer);
						}
						else if (serverEvent == 0) {
							cout << "maintenance = true now\n";
							serverMaintenance == 27;
							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;
								//enet_peer_disconnect_later(currentPeer, 0);
								if (((PlayerInfo*)(currentPeer->data))->rawName == "matty" || ((PlayerInfo*)(currentPeer->data))->rawName == "seth" || ((PlayerInfo*)(currentPeer->data))->rawName == "hamumu") {
									// do nothing
								}
								else {
									gamepacket_t p2;
									p2.Insert("OnConsoleMessage");
									p2.Insert("`4Global System Message:`o Server is now undergoing maintenance. We will be back shortly.");
									p2.CreatePacket(currentPeer);
									enet_peer_disconnect_later(currentPeer, 0);

								}
							}
						}
					}
                 }
					else if (str.substr(0,7) == "/event ") {
					sendSaveSet(peer);
					ENetPeer * currentPeer;
					int event = stoi(str.substr(7, str.length()));
					serverEvent = event;
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 900 || ((PlayerInfo*)(peer->data))->adminLevel > 900) {
						gamepacket_t p1;
						p1.Insert("OnConsoleMessage");
						p1.Insert("`oChanging Server event to `5" + std::to_string(event) + "`o.");
						p1.CreatePacket(peer);
						/*
-------Monthly Events -----------
0 = None, 1 = Anniversary Week, 2 = Lunar New Year, 3 = Valentines week, 4 = St. Patricks Week, 5 = Easter Week,
6 = Cinco De Mayo week, 7 = Super Pineapple Week, 8 = Summerfest, 9 = P.A.W, 10 = Harvest Festival,
11 = Wing Week, 12 = Halloween, 13 = Thanksgiving, 14 = Winterfest,
-------Weekly/Daily Events-------
15 = Carnival, 16 = Night of The Comet, 17 = Locke the Travling Salesman, 18 = The Grand Tournament,
19 = Surgery Day, 20 = All howl's eve, 21 = Geiger Day, 22 = Ghost Day, 23 = Mutant Kitchen
--------------OTHER--------------
24 = Apology Week/Weekend, 25 = NUL, 26 = Test
*/
						gamepacket_t p3;
						p3.Insert("OnConsoleMessage");
						if (serverEvent == 1) {
							p3.Insert("`2Growtopia is a year older! `oGet `480%`o more Gems for your money this week, and enjoy special party events!");
						}
						else if (serverEvent == 2) {
							p3.Insert("`2Happy Lunar New Year!`o It's the Year of the Ox! Open those fortune cookies and ");
						}
						else if (serverEvent == 3) {
							p3.Insert("`2It's Valentine's Week!`o Spend the time enjoying the company of your friends and sharing the love and peace of the universe, man!");
						}
						else if (serverEvent == 4) {
							p3.Insert("`2It's St Patricks Week!`o");
						}
						else if (serverEvent == 5) {
							p3.Insert("`2It's Easter Week!`o");
						}
						else if (serverEvent == 6) {
							p3.Insert("`2It's Cinco De Mayo week!`o");
						}
						else if (serverEvent == 7) {
							p3.Insert("`2It's Super Pineapple Week!`o");
						}
						else if (serverEvent == 8) {
							p3.Insert("`2IT'S SUMMERFEST! `oGrab your Water Wings and jump in the ocean! Have a barbecue and build a Sand Castle.");
						}
						else if (serverEvent == 9) {
							p3.Insert("`2Player Appreciation Week!`o Seven days of festivities and random new stuff suggested by players!");
						}
						else if (serverEvent == 10) {
							p3.Insert("`2HARVEST FESTIVAL! `oCelebrate harvest festivals from around the world with special events and items.Every time you harvest a tree, there is a small chance a Mooncake will pop out!");
						}
						else if (serverEvent == 11) {
							p3.Insert("`2Wing Week!`o Spread your wings and welcome the new annual event, Wing Week is here!");
						}
						else if (serverEvent == 12) {
							p3.Insert("`2It's Halloween!`o Put on a costume and knock on some doors, or if you're feeling bold, visit GROWGANOTH for a taste of pure swirl evil.");
						}
						else if (serverEvent == 13) {
							p3.Insert("`2Happy Thanksgiving!`o Thanksgiving Week is here, and turkeytastic treasures await!");
						}
						else if (serverEvent == 14) {
							p3.Insert("`2It's Winterfest!`o Presents will be appearing all over Growtopia - what will you do with yours? Be selfish and open it, or share?");
						}
						else if (serverEvent == 15) {
							p3.Insert("`2The Carnival has come to town! `oVisit CARNIVAL to play games and win unique prizes. But hurry, in a couple days they will move on.");
						}
						else if (serverEvent == 18) {
							p3.Insert("`2The Grand Tournament has come to Growtopia!`o See who's the champion this month in the world TOURNAMENT!");
						}
						else if (serverEvent == 26) {
							p3.Insert("`2What's that in the sky??`o A comet is blazing a trail through the night! It will only be here for 24 hours...");
						}
						else if (serverEvent == 24) {
							p3.Insert("`1It's Apology Week!`o Everybody who plays is`2 Lucky`o (more gems and seeds from smashing blocks), and you get `425%`o more Gems when you buy gems!");
						}
						else {
							p3.Insert("`2Goodbye!`o It's no longer a special week/day!");
						}
						string text = "action|play_sfx\nfile|audio/dialog_confirm.wav\ndelayMS|0\n";
						BYTE* data = new BYTE[5 + text.length()];
						BYTE zero = 0;
						int type = 3;
						memcpy(data, &type, 4);
						memcpy(data + 4, text.c_str(), text.length());
						memcpy(data + 4 + text.length(), &zero, 1);
						ENetPeer * currentPeer;

						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							//if (!((PlayerInfo*)(currentPeer->data))->radio)
								//continue;

							p3.CreatePacket(currentPeer);


							ENetPacket * packet2 = enet_packet_create(data,
								5 + text.length(),
								ENET_PACKET_FLAG_RELIABLE);

							enet_peer_send(currentPeer, 0, packet2);

							//enet_host_flush(server);
						}
					}
								
						
					}

					else if (str.substr(0, 6) == "/warp ") {
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 500 || ((PlayerInfo*)(peer->data))->adminLevel > 500) {
						sendSaveSet(peer);

						ENetPeer * currentPeer;

						PlayerInfo* p = ((PlayerInfo*)(peer->data));
						string worldname = str.substr(6, str.length());
						toUpperCase(worldname);

						if (worldname == "CON" || worldname == "PRN" || worldname == "AUX" || worldname == "NUL" || worldname == "COM1" || worldname == "COM2" || worldname == "COM3" || worldname == "COM4" || worldname == "COM5" || worldname == "COM6" || worldname == "COM7" || worldname == "COM8" || worldname == "COM9" || worldname == "LPT1" || worldname == "LPT2" || worldname == "LPT3" || worldname == "LPT4" || worldname == "LPT5" || worldname == "LPT6" || worldname == "LPT7" || worldname == "LPT8" || worldname == "LPT9" || worldname == "EXIT") {

						}
						sendPlayerToWorld(peer, (PlayerInfo*)(peer->data), worldname);
						WorldInfo info = worldDB.get(worldname);
						sendWorld(peer, &info);
						gamepacket_t p1;
						p1.Insert("OnTextOverlay");
						p1.Insert("`wMagically warping to `o" + worldname);
						p1.CreatePacket(peer);
					}

					}
					else if (str.substr(0, 6) == "/clear") {
					sendSaveSet(peer);
					if (isSuperAdmin(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) || ((PlayerInfo*)(peer->data))->adminLevel == 999) {

						WorldInfo* wrld = getPlyersWorld(peer);
						
						ENetPeer* currentPeer;
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							if (((PlayerInfo*)(currentPeer->data))->currentWorld == wrld->name)
							{
								string act = ((PlayerInfo*)(peer->data))->currentWorld;
								//WorldInfo info = worldDB.get(act);
								// sendWorld(currentPeer, &info);
								int x = 3040;
								int y = 736;



								for (int i = 0; i < world->width * world->height; i++)
								{
									if (world->items[i].foreground == 2) {
										//world->items[i].foreground =0;
										world->items[i].foreground = 0;
										world->items[i].background = 0;
									} 
									if (world->items[i].foreground == 14) {
										//world->items[i].foreground =0;
										world->items[i].foreground = 0;
										world->items[i].background = 0;
									}
									if (world->items[i].foreground == 10) {
										//world->items[i].foreground =0;
										world->items[i].foreground = 0;
										world->items[i].background = 0;
									}
									if (world->items[i].foreground == 4) {
										//world->items[i].foreground =0;
										world->items[i].foreground = 0;
										world->items[i].background = 0;
									}
									else {
									//do nothing
									}
									/*else if (world->items[i].foreground == 8) {

									}
									else if (world->items[i].foreground == 242) {

									}*/
									
								}
								
								sendPlayerLeave(currentPeer, (PlayerInfo*)(currentPeer->data));
								((PlayerInfo*)(peer->data))->currentWorld = "EXIT";
								sendWorldOffers(currentPeer);




							}

						}
					}
					}
					else if (str == "/nuke") {
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 500 || ((PlayerInfo*)(peer->data))->adminLevel > 500) {
						sendSaveSet(peer);
						string world = ((PlayerInfo*)(peer->data))->currentWorld;
						nukedworlds.push_back(world);

						gamepacket_t p;
						p.Insert("OnConsoleMessage");
						p.Insert("`oThis world has been `5successfully`o nuked.");
						p.CreatePacket(peer);

						gamepacket_t p1;
						p1.Insert("OnConsoleMessage");
						p1.Insert("`o>> `4" + world + "`4 was nuked from orbit`o. It's the only way to be sure. Play nice, everybody!");
						string text = "action|play_sfx\nfile|audio/bigboom.wav\ndelayMS|0\n";
						BYTE* data = new BYTE[5 + text.length()];
						BYTE zero = 0;
						int type = 3;
						memcpy(data, &type, 4);
						memcpy(data + 4, text.c_str(), text.length());
						memcpy(data + 4 + text.length(), &zero, 1);
						ENetPeer * currentPeer;
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;

							p1.CreatePacket(currentPeer);

							ENetPacket * packet2 = enet_packet_create(data,
								5 + text.length(),
								ENET_PACKET_FLAG_RELIABLE);

							enet_peer_send(currentPeer, 0, packet2);

							//enet_host_flush(server);
						}


					}
					 }
					else if (str == "/diafind" || str == "/diafind ") {
					sendSaveSet(peer);
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 500 || ((PlayerInfo*)(peer->data))->adminLevel > 500){
					gamepacket_t p;
					p.Insert("OnDialogRequest");
					p.Insert("add_label_with_icon|big|`wItem Finder``|left|6016|\nadd_textbox|Enter a word below to Find the item!|\nadd_text_input|item|Item Name||30|\nend_dialog|findid|Cancel|Find the item!|\nadd_quick_exit|\n");
					p.CreatePacket(peer);
                    }
					
}
					else if (str == "/register") {
					sendSaveSet(peer);
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 999 || ((PlayerInfo*)(peer->data))->adminLevel == 999) {
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`wGet a GrowID``|left|206|\n\nadd_spacer|small|\nadd_textbox|A `wGrowID `wmeans `oyou can use a name and password to logon from any device.|\nadd_spacer|small|\nadd_textbox|This `wname `owill be reserved for you and `wshown to other players`o, so choose carefully! `wNote`o please do not use `w'of Legend'`o as you won't be able to login.|\nadd_text_input|username|GrowID||30|\nadd_text_input|password|Password||100|\nadd_text_input|passwordverify|Password Verify||100|\nadd_textbox|Your `wemail address `owill only be used for account verification purposes and won't be spammed or shared. If you use a fake email, you'll never be able to recover or change your password.|\nadd_text_input|email|Email||100|\nend_dialog|register|Cancel|Get My GrowID!|\n");
						p.CreatePacket(peer);
					}
					else {
						gamepacket_t p;
						p.Insert("OnTalkBubble");
						p.Insert("`oYou already have a GrowID!!");
						p.CreatePacket(peer);

					}
					}
					else if (str == "/sdb") {
					sendSaveSet(peer);
						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 999 || ((PlayerInfo*)(peer->data))->adminLevel == 999) {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`2Super-Duper-Broadcast``|left|660|\n\nadd_spacer|small|\nadd_textbox|`oSuper Duper Broadcasts pop up a box with your message on `4every`o player's screen. The box even includes a button to visit your world!|\nadd_spacer||small|`oEnter up to 3 lines of text!``|\nadd_spacer|small|\nadd_text_input|sdb1|SDB Line 1||50|\nadd_spacer|small|\nadd_text_input|sdb2|SDB Line 2||50|\nadd_text_input|sdb3|SDB Line 3||50|\nend_dialog|super|Cancel|Send SDB!|\n");
						//	p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`wGet a GrowID``|left|206|\n\nadd_spacer|small|\nadd_textbox|A `wGrowID `wmeans `oyou can use a name and password to logon from any device.|\nadd_spacer|small|\nadd_textbox|This `wname `owill be reserved for you and `wshown to other players`o, so choose carefully!|\nadd_text_input|SDB1|SDB Line 1||30|\nadd_text_input|password|Password||100|\nadd_text_input|passwordverify|Password Verify||100|\nadd_textbox|Your `wemail address `owill only be used for account verification purposes and won't be spammed or shared. If you use a fake email, you'll never be able to recover or change your password.|\nadd_text_input|email|Email||100|\nend_dialog|super|Cancel|Get My GrowID!|\n");

							p.CreatePacket(peer);
						}
}
					else if (str == "/news"){
					sendSaveSet(peer);
						packet::dialog(peer, newslist);
					}
					else if (str.substr(0, 9) == "/cheatxp ") {
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 900 || ((PlayerInfo*)(peer->data))->adminLevel > 900) {
						((PlayerInfo*)(peer->data))->xp = atoi(str.substr(9).c_str());
						gamepacket_t p;
						p.Insert("OnConsoleMessage");
						p.Insert("`oYour xp is now:`5 " + std::to_string(((PlayerInfo*)(peer->data))->xp));
						p.CreatePacket(peer);
					}
}
					else if (str == "/loadnews"){
					sendSaveSet(peer);
						//if (!isSuperAdmin(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass)) break;
						loadnews();//To load news instead of close server and run it again
					}
					else if (str == "/shop") {
					/*gamepacket_t p;
					p.Insert("OnDialogRequest");
					p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`2Super-Duper-Broadcast``|left|660|\n\nadd_spacer|small|\nadd_textbox|`oSuper Duper Broadcasts pop up a box with your message on `4every`o player's screen. The box even includes a button to visit your world!|\nadd_spacer||small|`oEnter up to 3 lines of text!``|\nadd_spacer|small|\nadd_button|buylname|`9Purchase Legendary Name?|\nadd_text_input|sdb1|SDB Line 1||50|\nadd_spacer|small|\nadd_text_input|sdb2|SDB Line 2||50|\nadd_text_input|sdb3|SDB Line 3||50|\nend_dialog|shoper|Cancel|Send SDB!|\n");
					p.CreatePacket(peer);*/
					gamepacket_t p;
					p.Insert("OnDialogRequest");
					p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`2Growtopia`w Shop!|left|32|\nadd_spacer|small|\nadd_banner|interface/large/gui_store_title_gems.rttex|0|1|\nadd_spacer|small|\nadd_label_with_icon|small|`9Legendary Items|left|1794|\nadd_button|locks_menu|`9Purchase Legendary Items|\nadd_label_with_icon|small|`2Awesome stuff!|left|4992|\nadd_button|awesomestuffs|`2Purchase Awesome Stuff!|\nadd_spacer|small|\nadd_banner|interface/large/gui_shop_featured_header.rttex|0|1||\nadd_textbox|`oView `wFeatured Items`o below! Each will take you to a specific dialog for each `witem`o!|\nadd_spacer|small|\nadd_image_button|iotm_layout|interface/large/gazette/gazette_3columns_feature_btn01.rttex|3imageslayout|OPENSTORE|main/itemomonth|\nadd_image_button|glory_layout|interface/large/gazette/gazette_3columns_feature_btn03.rttex|3imageslayout|NOFLAGS|||\nend_dialog|shoper|Cancel|\n");
					p.CreatePacket(peer);
}
					else if (str.substr(0, 6) == "/nick ") {
					sendSaveSet(peer);
						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 499 || ((PlayerInfo*)(peer->data))->adminLevel > 499) {
							//* ((PlayerInfo*)(event.peer->data))->isNicked == true;
							string nam1e = "``" + str.substr(6, cch.length() - 6 - 1) + "``";
							((PlayerInfo*)(event.peer->data))->displayName = str.substr(6, cch.length() - 6 - 1);

							gamepacket_t p(0, ((PlayerInfo*)(peer->data))->netID);
							p.Insert("OnNameChanged");
							p.Insert(nam1e);
							ENetPeer * currentPeer;
							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;
								if (isHere(peer, currentPeer))
								{
									p.CreatePacket(currentPeer);
								}
							}
						}
						
					}
					
						else if (str.substr(0, 5) == "/gem ") //gem if u want flex with ur gems!
						{
						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 900 || ((PlayerInfo*)(peer->data))->adminLevel > 900) {
							sendSaveSet(peer);
							int gems = ((PlayerInfo*)(peer->data))->gems + atoi(str.substr(5).c_str());
							gamepacket_t p;
							p.Insert("OnSetBux");
							p.Insert(gems);
							p.CreatePacket(peer);
							((PlayerInfo*)(peer->data))->gems = ((PlayerInfo*)(peer->data))->gems + atoi(str.substr(5).c_str());
							continue;
						}
						
						//((PlayerInfo*)(peer->data))->gems
						/*gamepacket_t p;
						p.Insert("OnSetBux");
						p.Insert(((PlayerInfo*)(peer->data))->gems);
						p.CreatePacket(peer);*/
						}
					
						
					else if (str.substr(0, 6) == "/flag ") {
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 900 || ((PlayerInfo*)(peer->data))->adminLevel > 900) {
						sendSaveSet(peer);
						int flagid = atoi(str.substr(6).c_str());

						gamepacket_t p(0, ((PlayerInfo*)(peer->data))->netID);
						p.Insert("OnGuildDataChanged");
						p.Insert(1);
						p.Insert(2);
						p.Insert(flagid);
						p.Insert(3);

						ENetPeer * currentPeer;
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							if (isHere(peer, currentPeer))
							{
								p.CreatePacket(currentPeer);
							}
						}
					}
					}
					else if (str.substr(0, 9) == "/weather ") {
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 900 || ((PlayerInfo*)(peer->data))->adminLevel > 900) {
						sendSaveSet(peer);
						if (world->name != "ADMIN") {
							if (world->owner != "") {
								if (isSuperAdmin(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) || ((PlayerInfo*)(peer->data))->adminLevel > 900)

								{
									ENetPeer* currentPeer;
									WorldInfo * worldInfo;
									//	worldInfo->weather = atoi(str.substr(9).c_str());
									getPlyersWorld(peer)->weather = atoi(str.substr(9).c_str());
									for (currentPeer = server->peers;
										currentPeer < &server->peers[server->peerCount];
										++currentPeer)
									{
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
											continue;
										if (isHere(peer, currentPeer))
										{

											packet::consolemessage(peer, "`oPlayer `2" + ((PlayerInfo*)(peer->data))->displayName + "`o has just changed the world's weather!");

											gamepacket_t p;
											p.Insert("OnSetCurrentWeather");
											p.Insert(atoi(str.substr(9).c_str()));
											p.CreatePacket(currentPeer);
											continue;

										}
									}
								}
							}
						}
					}
						}
					else if (str == "/count"){
					sendSaveSet(peer);
						int count = 0;
						ENetPeer * currentPeer;
						string name = "";
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							count++;
							count - 1;//current player online
						}
						packet::consolemessage(peer, "There are "+std::to_string(count)+" people online out of 1024 limit.");
					}
					else if (str == "/awkwardevent") {
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 900 || ((PlayerInfo*)(peer->data))->adminLevel > 900) {
						sendSaveSet(peer);
						gamepacket_t p;
						p.Insert("OnAddNotification");
						p.Insert("interface/large/special_event.rttex");
						p.Insert("`2Wild Unicorn Hunt:`o You have `w30`o seconds to find an `#Awkward Friendly Unicorn`o before it escapes!");
						p.Insert("audio/empty.wav");
						p.Insert(0);

						/*	gamepacket_t p5(30000);
							p.Insert("OnAddNotification");
							p.Insert("interface/large/special_event.rttex");
							p.Insert("`2Beautiful Cyrstal!:`o No one found the `#Crystal Block Seed`o in time.");
							p.Insert("audio/loser.wav");
							p.Insert(0);*/
						ENetPeer * currentPeer;
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							if (isHere(peer, currentPeer))
							{
								p.CreatePacket(currentPeer);
								//p5.CreatePacket(currentPeer);

							}
						}
					}
					}
					else if (str == "/crystalseedevent") {
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 900 || ((PlayerInfo*)(peer->data))->adminLevel > 900) {
						sendSaveSet(peer);
						gamepacket_t p;
						p.Insert("OnAddNotification");
						p.Insert("interface/large/special_event.rttex");
						p.Insert("`2Beautiful Cyrstal!:`o You have `w30`o seconds to find and grab the `#Crystal Block Seed.");
						p.Insert("audio/slot_win.wav");
						p.Insert(0);

						/*	gamepacket_t p5(30000);
							p.Insert("OnAddNotification");
							p.Insert("interface/large/special_event.rttex");
							p.Insert("`2Beautiful Cyrstal!:`o No one found the `#Crystal Block Seed`o in time.");
							p.Insert("audio/loser.wav");
							p.Insert(0);*/
						ENetPeer * currentPeer;
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							if (isHere(peer, currentPeer))
							{
								p.CreatePacket(currentPeer);
								//p5.CreatePacket(currentPeer);

							}
						}
					}
					
					}
					else if (str.substr(0, 5) == "/sev ") {
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 900 || ((PlayerInfo*)(peer->data))->adminLevel > 900) {
						sendSaveSet(peer);
						string speci = "`2Special Event!:`o ";
						gamepacket_t p;
						p.Insert("OnAddNotification");
						p.Insert("interface/large/special_event.rttex");
						p.Insert(speci + str.substr(4, cch.length() - 4 - 1).c_str());
						p.Insert("audio/slot_win.wav");
						p.Insert(0);
						gamepacket_t p1;
						p1.Insert("OnConsoleMessage");
						p1.Insert(speci + str.substr(4, cch.length() - 4 - 1).c_str());

						ENetPeer * currentPeer;
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							if (isHere(peer, currentPeer))
							{
								p.CreatePacket(currentPeer);
								p1.CreatePacket(currentPeer);

							}
						}
					}
					}
					else if (str.substr(0, 5) == "/asb "){
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 900 || ((PlayerInfo*)(peer->data))->adminLevel > 900) {
						sendSaveSet(peer);
						string warn = "`oWarning from `4Admin:`o";
						gamepacket_t p;
						p.Insert("OnAddNotification");
						p.Insert("interface/atomic_button.rttex");
						p.Insert(warn + str.substr(4, cch.length() - 4 - 1).c_str());
						p.Insert("audio/hub_open.wav");
						p.Insert(0);
						gamepacket_t p1;
						p1.Insert("OnConsoleMessage");
						p1.Insert(warn + str.substr(4, cch.length() - 4 - 1).c_str());

						ENetPeer * currentPeer;
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							p.CreatePacket(currentPeer);
							p1.CreatePacket(currentPeer);
						}
					}
					}
					else if (str == "/invis" || str == "/invisible") {

					ENetPeer * currentPeer;
						//if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 499 || ((PlayerInfo*)(peer->data))->adminLevel > 499) {
							packet::consolemessage(peer, "`6" + str);
							if (!pData->isGhost) {
								((PlayerInfo*)(peer->data))->back_actual = ((PlayerInfo*)(peer->data))->cloth_back;
								((PlayerInfo*)(peer->data))->face_actual = ((PlayerInfo*)(peer->data))->cloth_face;
								((PlayerInfo*)(peer->data))->feet_actual = ((PlayerInfo*)(peer->data))->cloth_feet;
								((PlayerInfo*)(peer->data))->hair_actual = ((PlayerInfo*)(peer->data))->cloth_hair;
								((PlayerInfo*)(peer->data))->hand_actual = ((PlayerInfo*)(peer->data))->cloth_hand;
								((PlayerInfo*)(peer->data))->mask_actual = ((PlayerInfo*)(peer->data))->cloth_mask;
								((PlayerInfo*)(peer->data))->necklace_actual = ((PlayerInfo*)(peer->data))->cloth_necklace;
								((PlayerInfo*)(peer->data))->pants_actual = ((PlayerInfo*)(peer->data))->cloth_pants;
								((PlayerInfo*)(peer->data))->shirt_actual = ((PlayerInfo*)(peer->data))->cloth_shirt;

								sendSaveSet(peer);
								packet::consolemessage(peer, "`oYou are now a ninja, invisible to all.");
								pData->isGhost = true;
								((PlayerInfo*)(peer->data))->isInvisible = true;
								string text = "action|play_sfx\nfile|audio/magic.wav\ndelayMS|0\n";
								BYTE* data = new BYTE[5 + text.length()];
								BYTE zero = 0;
								int type = 3;
								memcpy(data, &type, 4);
								memcpy(data + 4, text.c_str(), text.length());
								memcpy(data + 4 + text.length(), &zero, 1);
								ENetPacket* packet2 = enet_packet_create(data,
									5 + text.length(),
									ENET_PACKET_FLAG_RELIABLE);

								
								
								/*gamepacket_t p(0, ((PlayerInfo*)(peer->data))->netID);
								p.Insert("OnSetPos");
								p.Insert(pData->x, pData->y);
								p.CreatePacket(peer);*/
								
								((PlayerInfo*)(peer->data))->cloth_back = 0;
								((PlayerInfo*)(peer->data))->cloth_face = 0;
								((PlayerInfo*)(peer->data))->cloth_feet = 0;
								((PlayerInfo*)(peer->data))->cloth_hair = 0;
								((PlayerInfo*)(peer->data))->cloth_hand = 0;
								((PlayerInfo*)(peer->data))->cloth_mask = 0;
								((PlayerInfo*)(peer->data))->cloth_necklace = 0;
								((PlayerInfo*)(peer->data))->cloth_pants = 0;
								((PlayerInfo*)(peer->data))->cloth_shirt = 0;
								//((PlayerInfo*)(peer->data))->skinColor = 2;

								sendState(peer);
								sendClothes(peer);
								sendClothes(peer);
								int counts = 0;
								counts = 0 - 1;
								for (currentPeer = server->peers;
									currentPeer < &server->peers[server->peerCount];
									++currentPeer)
								{
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
										continue;
									if (isHere(peer, currentPeer))
									{
										counts++;
										//Taking away one from the count to account for the currentplayer. :)
										counts - 1;

									}
								}

								string pname = ((PlayerInfo*)(peer->data))->displayName;
								PlayerInfo* player;
								gamepacket_t p2;
								p2.Insert("OnConsoleMessage");
								p2.Insert("`5<`w" + pname + "`5 left, `w" + std::to_string(counts) + "`5 others here>``");
								for (currentPeer = server->peers;
									currentPeer < &server->peers[server->peerCount];
									++currentPeer)
								{
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
										continue;
									if (isHere(peer, currentPeer)) {
										{
											//p.CreatePacket(peer);

											{
												//p.CreatePacket(currentPeer);
											}

										}
										{
											enet_peer_send(currentPeer, 0, packet2);
											p2.CreatePacket(currentPeer);
										}
									}
								}
							}
							else if (pData->isGhost){
								PlayerInfo* player;
								ENetPeer * currentPeer;
								int counts = 0;
								counts = 0 - 1;
								for (currentPeer = server->peers;
									currentPeer < &server->peers[server->peerCount];
									++currentPeer)
								{
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
										continue;
									if (isHere(peer, currentPeer))
									{
										counts++;
										//Taking away one from the count to account for the currentplayer. :)
										counts - 1;

									}
								}
								string pname = ((PlayerInfo*)(peer->data))->displayName;
								packet::consolemessage(peer, "`oYour atoms return to normal. Visible to all.");

								/*gamepacket_t p7(0, ((PlayerInfo*)(peer->data))->netID);
								p7.Insert("OnSetPos");
								p7.Insert(pData->x1, pData->y1);
								p7.CreatePacket(peer);*/

								((PlayerInfo*)(peer->data))->isInvisible = true;
								string text = "action|play_sfx\nfile|audio/magic.wav\ndelayMS|0\n";
								BYTE* data = new BYTE[5 + text.length()];
								BYTE zero = 0;
								int type = 3;
								memcpy(data, &type, 4);
								memcpy(data + 4, text.c_str(), text.length());
								memcpy(data + 4 + text.length(), &zero, 1);
								ENetPacket* packet2 = enet_packet_create(data,
									5 + text.length(),
									ENET_PACKET_FLAG_RELIABLE);

								

								((PlayerInfo*)(peer->data))->isInvisible = false;
								
								((PlayerInfo*)(peer->data))->skinColor = -2104114177;
								sendState(peer);
								sendClothes(peer);
								pData->isGhost = false;
								gamepacket_t enter;
								enter.Insert("OnConsoleMessage");
                                enter.Insert("`5<`w" + pname + "`5 entered, `w" + std::to_string(counts) + "`5 others here>``");
								PlayerInfo* pData = ((PlayerInfo*)(peer->data));
								((PlayerInfo*)(peer->data))->hidden = false;

								string name = ((PlayerInfo*)(peer->data))->rawName;
			
								((PlayerInfo*)(peer->data))->cloth_back = ((PlayerInfo*)(peer->data))->back_actual;
								((PlayerInfo*)(peer->data))->cloth_face = ((PlayerInfo*)(peer->data))->face_actual;
								((PlayerInfo*)(peer->data))->cloth_feet = ((PlayerInfo*)(peer->data))->feet_actual;
								((PlayerInfo*)(peer->data))->cloth_hair = ((PlayerInfo*)(peer->data))->hair_actual;
								((PlayerInfo*)(peer->data))->cloth_hand = ((PlayerInfo*)(peer->data))->hand_actual;
								((PlayerInfo*)(peer->data))->cloth_mask = ((PlayerInfo*)(peer->data))->mask_actual;
								((PlayerInfo*)(peer->data))->cloth_necklace = ((PlayerInfo*)(peer->data))->necklace_actual;
								((PlayerInfo*)(peer->data))->cloth_pants = ((PlayerInfo*)(peer->data))->pants_actual;
								((PlayerInfo*)(peer->data))->cloth_shirt = ((PlayerInfo*)(peer->data))->shirt_actual;
								
								sendState(peer);
								sendClothes(peer);

								for (currentPeer = server->peers;
									currentPeer < &server->peers[server->peerCount];
									++currentPeer)
								{
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
										continue;
									if (isHere(peer, currentPeer))
									{
										enter.CreatePacket(currentPeer);
										sendState(currentPeer);
										sendClothes(currentPeer);
										enet_peer_send(currentPeer, 0, packet2);

									}
								}
							}
						
					}
					
					else if (str == "/magic"){
					sendSaveSet(peer);
					string text = "action|play_sfx\nfile|audio/magic.wav\ndelayMS|0\n";
					BYTE* data = new BYTE[5 + text.length()];
					BYTE zero = 0;
					int type = 3;
					memcpy(data, &type, 4);
					memcpy(data + 4, text.c_str(), text.length());
					memcpy(data + 4 + text.length(), &zero, 1);
					ENetPeer * currentPeer;
					if (((PlayerInfo*)(peer->data))->isInvisible == false) {
						((PlayerInfo*)(peer->data))->isInvisible = true;
						packet::consolemessage(peer, "`oYou are now a ninja, invisible to all.");
						sendState(peer);
						sendClothes(peer);
					} else if (((PlayerInfo*)(peer->data))->isInvisible == true) {
						((PlayerInfo*)(peer->data))->isInvisible = false;
						packet::consolemessage(peer, "`oYou are now a ninja, invisible to all.");
						sendState(peer);
						sendClothes(peer);
					}
					//((PlayerInfo*)(peer->data))->isInvisible = false;
					sendState(peer);
					sendClothes(peer);
					
					/*gamepacket_t p(0, ((PlayerInfo*)(peer->data))->netID);
					p.Insert("OnSetPos");
					p.Insert(pData->x1, pData->y1);
					p.CreatePacket(peer);*/
					//Player::OnParticleEffect(currentPeer, effect, x, y, 0);

					int effect = 105;
					int x = static_cast<PlayerInfo*>(peer->data)->x;
					int y = static_cast<PlayerInfo*>(peer->data)->y;
					gamepacket_t magic;
					magic.Insert("OnParticleEffect");
					magic.Insert(effect);
					magic.Insert(x + 5, y + 5, 0);
					gamepacket_t magic1;
					magic.Insert("OnParticleEffect");
					magic.Insert(effect);
					magic.Insert(x + 15, y + 15, 0);
					gamepacket_t magic2;
					magic.Insert("OnParticleEffect");
					magic.Insert(effect);
					magic.Insert(x + 10, y + 10, 0);
					/*magic.Insert(x);
					magic.Insert(y);
					magic.Insert(0);*/


					((PlayerInfo*)(peer->data))->isGhost = false;
//					ENetPeer * currentPeer;
					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							ENetPacket * packet2 = enet_packet_create(data,
								5 + text.length(),
								ENET_PACKET_FLAG_RELIABLE);

							enet_peer_send(currentPeer, 0, packet2);
							magic.CreatePacket(currentPeer);
							magic1.CreatePacket(currentPeer);
							magic2.CreatePacket(currentPeer);
						}
					}

					 }
					
					else if (str.substr(0, 8) == "/effect ") {
					sendSaveSet(peer);
					ENetPeer * currentPeer;
					int effect = atoi(str.substr(8, cch.length() - 8 - 1).c_str());
					int x = static_cast<PlayerInfo*>(peer->data)->x;
					int y = static_cast<PlayerInfo*>(peer->data)->y;
					gamepacket_t magic;
					magic.Insert("OnParticleEffect");
					magic.Insert(effect);
					magic.Insert(x, y, 0);
					/*magic.Insert(x);
					magic.Insert(y);
					magic.Insert(0);*/
					
					for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;
						if (isHere(peer, currentPeer))
						{
							
							magic.CreatePacket(currentPeer);

						}
					}
                          }
					else if (str.substr(0, 4) == "/sb ") {
					sendSaveSet(peer);
					using namespace std::chrono;
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 495 || ((PlayerInfo*)(peer->data))->adminLevel > 495) {
						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 999 || ((PlayerInfo*)(peer->data))->adminLevel == 999) {
							time_t now = time(0);
							tm *ltm = localtime(&now);
							string name = ((PlayerInfo*)(peer->data))->displayName;
							cout << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "]" << " JSB from " + name + ". Message: " + str.substr(4, cch.length() - 4 - 1) + "\n";
							gamepacket_t p;
							p.Insert("OnConsoleMessage");
							p.Insert("CP:0_PL:4_OID:_CT:[SB]_ `5** `5from (`2" + name + "`5) in [`4JAMMED!`5] ** :`` `5 " + str.substr(4, cch.length() - 4 - 1));
							//cout << "Jammed SB sent by:" + name + "Message contained: " + str.substr(5, cch.length() - 5 - 1) + "\n";
							string text = "action|play_sfx\nfile|audio/beep.wav\ndelayMS|0\n";
							BYTE* data = new BYTE[5 + text.length()];
							BYTE zero = 0;
							int type = 3;
							memcpy(data, &type, 4);
							memcpy(data + 4, text.c_str(), text.length());
							memcpy(data + 4 + text.length(), &zero, 1);
							ENetPeer * currentPeer;

							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;
								if (!((PlayerInfo*)(currentPeer->data))->radio)
									continue;

								p.CreatePacket(currentPeer);


								ENetPacket * packet2 = enet_packet_create(data,
									5 + text.length(),
									ENET_PACKET_FLAG_RELIABLE);

								enet_peer_send(currentPeer, 0, packet2);

								//enet_host_flush(server);
							}
							delete[] data;
						}
						else if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 500 || ((PlayerInfo*)(peer->data))->adminLevel == 500) {
							string name = ((PlayerInfo*)(peer->data))->displayName;
							gamepacket_t p;
							p.Insert("OnConsoleMessage");
							p.Insert("CP:0_PL:4_OID:_CT:[SB]_ `5** `5from (`2" + name + "`5) in [`4JAMMED!`5] ** :`` `^ " + str.substr(4, cch.length() - 4 - 1));
							//cout << "Jammed SB sent by:" + name + "Message contained: " + str.substr(5, cch.length() - 5 - 1) + "\n";
							string text = "action|play_sfx\nfile|audio/beep.wav\ndelayMS|0\n";
							BYTE* data = new BYTE[5 + text.length()];
							BYTE zero = 0;
							int type = 3;
							memcpy(data, &type, 4);
							memcpy(data + 4, text.c_str(), text.length());
							memcpy(data + 4 + text.length(), &zero, 1);
							ENetPeer * currentPeer;

							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;
								if (!((PlayerInfo*)(currentPeer->data))->radio)
									continue;

								p.CreatePacket(currentPeer);


								ENetPacket * packet2 = enet_packet_create(data,
									5 + text.length(),
									ENET_PACKET_FLAG_RELIABLE);

								enet_peer_send(currentPeer, 0, packet2);

								//enet_host_flush(server);
							}
							delete[] data;

						}
					}
					else {
						if (((PlayerInfo*)(peer->data))->lastSB + 45000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count())
						{
							((PlayerInfo*)(peer->data))->lastSB = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
						}
						else {
							packet::consolemessage(peer, "Wait a minute before using the SB command again!");
							continue;
						}

						string name = ((PlayerInfo*)(peer->data))->displayName;
						gamepacket_t p;
						p.Insert("OnConsoleMessage");
						p.Insert("CP:0_PL:4_OID:_CT:[SB]_ `5** from (`2" + name + "`5) `5[`$" + ((PlayerInfo*)(peer->data))->currentWorld + "`5] ** :`` `$ " + str.substr(4, cch.length() - 4 - 1));

						string text = "action|play_sfx\nfile|audio/beep.wav\ndelayMS|0\n";
						BYTE* data = new BYTE[5 + text.length()];
						BYTE zero = 0;
						int type = 3;
						memcpy(data, &type, 4);
						memcpy(data + 4, text.c_str(), text.length());
						memcpy(data + 4 + text.length(), &zero, 1);
						ENetPeer * currentPeer;

						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							if (!((PlayerInfo*)(currentPeer->data))->radio)
								continue;

							p.CreatePacket(currentPeer);

							ENetPacket * packet2 = enet_packet_create(data,
								5 + text.length(),
								ENET_PACKET_FLAG_RELIABLE);

							enet_peer_send(currentPeer, 0, packet2);

							//enet_host_flush(server);
						}
						delete[] data;
					}
					
					}
					else if (str.substr(0, 4) == "/mc ") {
					using namespace std::chrono;
					
					string name = ((PlayerInfo*)(peer->data))->displayName;
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 495 || ((PlayerInfo*)(peer->data))->adminLevel > 495) {
						gamepacket_t p;
						p.Insert("OnConsoleMessage");
						p.Insert("`2[`9MOD-CHAT`2] from `e" + name + "`2 :`` `5 " + str.substr(4, cch.length() - 4 - 1));
						//cout << "[" << ltm->tm_hour << ":" << 30 + ltm->tm_min << ":" << ltm->tm_sec << "]" << " Normal SB sent by:" + name + "Message contained: " + str.substr(5, cch.length() - 5 - 1) + "\n";
						string text = "action|play_sfx\nfile|audio/gem_pickup.wav\ndelayMS|0\n";
						BYTE* data = new BYTE[5 + text.length()];
						BYTE zero = 0;
						int type = 3;
						memcpy(data, &type, 4);
						memcpy(data + 4, text.c_str(), text.length());
						memcpy(data + 4 + text.length(), &zero, 1);
						ENetPeer * currentPeer;
						
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 495 && getAdminLevel(((PlayerInfo*)(currentPeer->data))->rawName, ((PlayerInfo*)(currentPeer->data))->tankIDPass) > 495 || ((PlayerInfo*)(peer->data))->adminLevel > 495 && ((PlayerInfo*)(currentPeer->data))->adminLevel > 495) {
								p.CreatePacket(currentPeer);

								ENetPacket * packet2 = enet_packet_create(data,
									5 + text.length(),
									ENET_PACKET_FLAG_RELIABLE);

								enet_peer_send(currentPeer, 0, packet2);

								//enet_host_flush(server);
							}
							
						}
						delete[] data;

					}
					}
					/*else if (str.substr(0, 5) == "/jsb ") {

					
						using namespace std::chrono;

						if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 999) {
							time_t now = time(0);
							tm *ltm = localtime(&now);
							string name = ((PlayerInfo*)(peer->data))->displayName;
							cout << "[" << ltm->tm_hour << ":" << 30 + ltm->tm_min << ":" << ltm->tm_sec << "]" << " JSB from " + name + ". Message: " + str.substr(5, cch.length() - 5 - 1) + "\n";
							gamepacket_t p;
							p.Insert("OnConsoleMessage");
							p.Insert("CP:0_PL:4_OID:_CT:[SB]_ `w** `5Super-Broadcast`` from `$`2" + name + "```` (in `4JAMMED``) ** :`` `5 " + str.substr(5, cch.length() - 5 - 1));
							//cout << "Jammed SB sent by:" + name + "Message contained: " + str.substr(5, cch.length() - 5 - 1) + "\n";
							string text = "action|play_sfx\nfile|audio/beep.wav\ndelayMS|0\n";
							BYTE* data = new BYTE[5 + text.length()];
							BYTE zero = 0;
							int type = 3;
							memcpy(data, &type, 4);
							memcpy(data + 4, text.c_str(), text.length());
							memcpy(data + 4 + text.length(), &zero, 1);
							ENetPeer * currentPeer;

							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;
								if (!((PlayerInfo*)(currentPeer->data))->radio)
									continue;

								p.CreatePacket(currentPeer);


								ENetPacket * packet2 = enet_packet_create(data,
									5 + text.length(),
									ENET_PACKET_FLAG_RELIABLE);

								enet_peer_send(currentPeer, 0, packet2);

								//enet_host_flush(server);
							}
							delete[] data;
						}
						else if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 500) {
							string name = ((PlayerInfo*)(peer->data))->displayName;
							gamepacket_t p;
							p.Insert("OnConsoleMessage");
							p.Insert("CP:0_PL:4_OID:_CT:[SB]_ `w** `5Super-Broadcast`` from `$`2" + name + "```` (in `4JAMMED``) ** :`` `^ " + str.substr(5, cch.length() - 5 - 1));
							//cout << "Jammed SB sent by:" + name + "Message contained: " + str.substr(5, cch.length() - 5 - 1) + "\n";
							string text = "action|play_sfx\nfile|audio/beep.wav\ndelayMS|0\n";
							BYTE* data = new BYTE[5 + text.length()];
							BYTE zero = 0;
							int type = 3;
							memcpy(data, &type, 4);
							memcpy(data + 4, text.c_str(), text.length());
							memcpy(data + 4 + text.length(), &zero, 1);
							ENetPeer * currentPeer;

							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;
								if (!((PlayerInfo*)(currentPeer->data))->radio)
									continue;

								p.CreatePacket(currentPeer);


								ENetPacket * packet2 = enet_packet_create(data,
									5 + text.length(),
									ENET_PACKET_FLAG_RELIABLE);

								enet_peer_send(currentPeer, 0, packet2);

								//enet_host_flush(server);
							}
							delete[] data;

						}
					}*/
					else if (str.substr(0, 5) == "/gsm ") {
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 900 || ((PlayerInfo*)(peer->data))->adminLevel > 900) {

						sendSaveSet(peer);
						using namespace std::chrono;

						string name = ((PlayerInfo*)(peer->data))->displayName;
						gamepacket_t p;
						p.Insert("OnConsoleMessage");
						p.Insert("`o**`$ Global System Message:`5 " + str.substr(5, cch.length() - 5 - 1));
						cout << "Global System Message sent by:" + name + "Message contained: " + str.substr(5, cch.length() - 5 - 1) + "\n";
						string text = "action|play_sfx\nfile|audio/dialog_confirm.wav\ndelayMS|0\n";
						BYTE* data = new BYTE[5 + text.length()];
						BYTE zero = 0;
						int type = 3;
						memcpy(data, &type, 4);
						memcpy(data + 4, text.c_str(), text.length());
						memcpy(data + 4 + text.length(), &zero, 1);
						ENetPeer * currentPeer;

						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							//if (!((PlayerInfo*)(currentPeer->data))->radio)
								//continue;

							p.CreatePacket(currentPeer);


							ENetPacket * packet2 = enet_packet_create(data,
								5 + text.length(),
								ENET_PACKET_FLAG_RELIABLE);

							enet_peer_send(currentPeer, 0, packet2);

							//enet_host_flush(server);
						}
						delete data;
					}
					}
					
					
					else if (str.substr(0, 6) == "/radio") {
					sendSaveSet(peer);
						gamepacket_t p;
						if (((PlayerInfo*)(peer->data))->radio) {
							p.Insert("OnConsoleMessage");
							p.Insert("You won't see broadcasts anymore.");
							((PlayerInfo*)(peer->data))->radio = false;
						} else {
							p.Insert("OnConsoleMessage");
							p.Insert("You will now see broadcasts again.");
							((PlayerInfo*)(peer->data))->radio = true;
						}
						p.CreatePacket(peer);
					}
					else if (str == "/restart") {
					sendSaveSet(peer);
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 999 || ((PlayerInfo*)(peer->data))->adminLevel == 999){
						gamepacket_t p22;
						p22.Insert("OnConsoleMessage");
						p22.Insert("`4Global System Message``: `oRestarting server for update in `41 ``minute");
						string text = "action|play_sfx\nfile|audio/ogg/suspended.ogg\ndelayMS|0\n";
						BYTE* data = new BYTE[5 + text.length()];
						BYTE zero = 0;
						int type = 3;
						memcpy(data, &type, 4);
						memcpy(data + 4, text.c_str(), text.length());
						memcpy(data + 4 + text.length(), &zero, 1);
						gamepacket_t p5(30000);
						p5.Insert("OnConsoleMessage");
						p5.Insert("`4Global System Message``: Restarting server for update in `430 ``seconds");

						gamepacket_t p7(50000);
						p7.Insert("OnConsoleMessage");
						p7.Insert("`4Global System Message``: Restarting server for update in `410 ``seconds");


						gamepacket_t p17(60000);
						p17.Insert("OnConsoleMessage");
						p17.Insert("`4Global System  Message``: Restarting server for update in `4ZERO ``seconds! Should be back up in a minute or so. BYE!");
						
						
						ENetPeer * currentPeer;
						for (currentPeer = server->peers;
							currentPeer < &server->peers[server->peerCount];
							++currentPeer)
						{
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
								continue;
							
							p22.CreatePacket(currentPeer);
							p5.CreatePacket(currentPeer);
							p7.CreatePacket(currentPeer);
							p17.CreatePacket(currentPeer);
							ENetPacket * packet2 = enet_packet_create(data,
								5 + text.length(),
								ENET_PACKET_FLAG_RELIABLE);

							enet_peer_send(currentPeer, 0, packet2);
						}

}
					
					}
					else if (str == "/beta") {
					sendSaveSet(peer);
					gamepacket_t p;
					p.Insert("OnSetBetaMode");
					p.Insert(1);
					}

					else if (str == "/cinv") {
					sendSaveSet(peer);
					PlayerInventory inventory = ((PlayerInfo *)(peer->data))->inventory;
					((PlayerInfo*)(event.peer->data))->inventory.items.clear();
					InventoryItem item1, item2;
					int itemPunch = 18;
					int itemWrench = 32;
				    item1.itemID = itemPunch;
					item2.itemID = itemWrench;
					item1.itemCount = 1;
					item2.itemCount = 1;
					inventory.items.push_back(item1);
					inventory.items.push_back(item2);
					sendInventory(peer, inventory);

                      }
					else
					if (str == "/inventory")
					{
						sendInventory(peer, ((PlayerInfo*)(peer->data))->inventory);
					} else
					if (str.substr(0,6) == "/give ")
					{
						sendSaveSet(peer);
						PlayerInventory inventory = ((PlayerInfo *)(peer->data))->inventory;
						InventoryItem item, item1, item2;
						ItemDefinition def;
						int itemID = atoi(str.substr(6, cch.length() - 6 - 1).c_str());
						int itemPunch = 18;
						int itemWrench = 32;
						if (itemID != 112 && itemID != 18 && itemID != 32) {
							item.itemID = itemID;
							item1.itemID = itemPunch;
							item2.itemID = itemWrench;
							item.itemCount = 200;
							item1.itemCount = 1;
							item2.itemCount = 1;

							inventory.items.push_back(item);
							inventory.items.push_back(item1);
							inventory.items.push_back(item2);
							
							sendInventory(peer, inventory);
						
							//sendSaveInventory(peer);
						}
						def = getItemDef(itemID);
						packet::consolemessage(peer, "`6/give " + to_string(itemID));
						packet::consolemessage(peer, "`oGiven `w200 " + def.name + "`o. Rarity: `w" + to_string(def.rarity));
						packet::consolemessage(peer, "`6>> Gave 200 " + def.name + "`o. Rarity: `w" + to_string(def.rarity) + "`o. To User: " + ((PlayerInfo*)(peer->data))->displayName);
					
					} 
					else if (str.substr(0, 8) == "/calvin ") {
						ItemDefinition def;
						int itemID = atoi(str.substr(8, cch.length() - 8 - 1).c_str());


						def = getItemDef(itemID);
						packet::consolemessage(peer, "`rItem ID : " + to_string(itemID) + " is name " + def.name + " `oRarity:`w " + to_string(def.rarity));
					}
					else if (str.substr(0, 6) == "/team ")
					{
						sendSaveSet(peer);
						int val = 0;
						val = atoi(str.substr(6, cch.length() - 6 - 1).c_str());
						PlayerMoving data;
						//data.packetType = 0x14;
						data.packetType = 0x1B;
						//data.characterState = 0x924; // animation
						data.characterState = 0x0; // animation
						data.x = 0;
						data.y = 0;
						data.punchX = val;
						data.punchY = 0;
						data.XSpeed = 0;
						data.YSpeed = 0;
						data.netID = ((PlayerInfo*)(peer->data))->netID;
						data.plantingTree = 0;
						SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);

					} else 
					if (str.substr(0, 7) == "/color ")
					{
						sendSaveSet(peer);
						((PlayerInfo*)(peer->data))->skinColor = atoi(str.substr(6, cch.length() - 6 - 1).c_str());
						
						sendClothes(peer);
					}
					else if (str.substr(0, 4) == "/who")
					{
						sendSaveSet(peer);
						sendWho(peer);

					}
					else {
						packet::consolemessage(peer, "`4Unknown command.`o Enter /help for a list of valid commands.");
					}
					if (str.length() && str[0] == '/')
					{
						sendAction(peer, ((PlayerInfo*)(peer->data))->netID, str);
					} else if (str.length()>0)
					{
						if (((PlayerInfo*)(peer->data))->taped == false) {
							
							sendChatMessage(peer, ((PlayerInfo*)(peer->data))->netID, str);
							
							
						}
						else {
							// Is duct-taped
							sendChatMessage(peer, ((PlayerInfo*)(peer->data))->netID, randomDuctTapeMessage(str.length()));
						}
					}
					
			}
				if (!((PlayerInfo*)(event.peer->data))->isIn)
				{
					
					if (itemdathash == 0) {
						enet_peer_disconnect_later(peer, 0);
					}
					
					gamepacket_t p;
					p.Insert("OnSuperMainStartAcceptLogonHrdxs47254722215a");
					p.Insert(itemdathash);
					p.Insert("ubistatic-a.akamaihd.net");
					//p.Insert("127.0.0.1");
					p.Insert(configCDN);
					p.Insert("cc.cz.madkite.freedom org.aqua.gg idv.aqua.bulldog com.cih.gamecih2 com.cih.gamecih com.cih.game_cih cn.maocai.gamekiller com.gmd.speedtime org.dax.attack com.x0.strai.frep com.x0.strai.free org.cheatengine.cegui org.sbtools.gamehack com.skgames.traffikrider org.sbtoods.gamehaca com.skype.ralder org.cheatengine.cegui.xx.multi1458919170111 com.prohiro.macro me.autotouch.autotouch com.cygery.repetitouch.free com.cygery.repetitouch.pro com.proziro.zacro com.slash.gamebuster");
					p.Insert("proto=136|choosemusic=audio/mp3/about_theme.mp3|active_holiday=0|wing_week_day=2|server_tick=6831222|clash_active=1|drop_lavacheck_faster=1|isPayingUser=1|usingStoreNavigation=1|enableInventoryTab=1|bigBackpack=1|");
					//old proto is 84
					p.CreatePacket(peer);

					if ("active_holday" == std::to_string(3)) {
						cout << "active holiday is 3\n";
					}

					std::stringstream ss(GetTextPointerFromPacket(event.packet));
					std::string to;
					while (std::getline(ss, to, '\n')){
						string id = to.substr(0, to.find("|"));
						string act = to.substr(to.find("|") + 1, to.length() - to.find("|") - 1);
						if (id == "tankIDName")
						{
							//if (((PlayerInfo*)(event.peer->data))->tankIDName == ""){}
					
								((PlayerInfo*)(event.peer->data))->tankIDName = act;
								((PlayerInfo*)(event.peer->data))->haveGrowId = true;
							
						}
						else if(id == "tankIDPass")
						{
							((PlayerInfo*)(event.peer->data))->tankIDPass = act;
						}
						else if(id == "requestedName")
						{
							((PlayerInfo*)(event.peer->data))->requestedName = act;
						}
						else if (id == "country")
						{
							((PlayerInfo*)(event.peer->data))->country = act;
						}
						else if (id == "game_version") {
							((PlayerInfo*)(event.peer->data))->gameversion = act;
						}
						else if (id == "platformID") {
							((PlayerInfo*)(event.peer->data))->platformID = act;
						}
						else if (id == "deviceVersion")
						{
							((PlayerInfo*)(event.peer->data))->deviceversion = act;
						}
						else if (id == "rid")
						{

							((PlayerInfo*)(event.peer->data))->rid = act;
						}
					}
					if (!((PlayerInfo*)(event.peer->data))->haveGrowId)
					{
						if (serverEvent == 27) {
							maintlog(peer);
						}
						else {
							int num;
							int rancount = 1;
							unsigned seed = time(0);

							srand(seed);

							while (rancount <= 20)
							{
								num = 100 + rand() % 899;

								rancount++;
							}
							((PlayerInfo*)(event.peer->data))->hasLogon = true;
							//Setting the Raw name to guest name so that Moderators/Developers can punish the guest account if needing to.
							((PlayerInfo*)(event.peer->data))->rawName = PlayerDB::fixColors(((PlayerInfo*)(event.peer->data))->requestedName.substr(0, ((PlayerInfo*)(event.peer->data))->requestedName.length() > 15 ? 15 : ((PlayerInfo*)(event.peer->data))->requestedName.length())) + "_" + std::to_string(num);
							((PlayerInfo*)(event.peer->data))->displayName = PlayerDB::fixColors(((PlayerInfo*)(event.peer->data))->requestedName.substr(0, ((PlayerInfo*)(event.peer->data))->requestedName.length() > 15 ? 15 : ((PlayerInfo*)(event.peer->data))->requestedName.length())) + "_" + std::to_string(num);

							if (((PlayerInfo*)(event.peer->data))->charIP == "127.0.0.1" && ((PlayerInfo*)(event.peer->data))->haveGrowId == false) {
								((PlayerInfo*)(event.peer->data))->displayName = "`6@" + PlayerDB::fixColors(((PlayerInfo*)(event.peer->data))->requestedName.substr(0, ((PlayerInfo*)(event.peer->data))->requestedName.length() > 15 ? 15 : ((PlayerInfo*)(event.peer->data))->requestedName.length())) + "_" + std::to_string(num);
								((PlayerInfo*)(event.peer->data))->adminLevel = 999;
								//	admin.level == 999;
							}
						}
						
					}
					else {
						((PlayerInfo*)(event.peer->data))->rawName = PlayerDB::getProperName(((PlayerInfo*)(event.peer->data))->tankIDName);
#ifdef REGISTRATION/*if (((PlayerInfo*)(peer->data))->isSuspended == true)
			{
				cout << "suspended" << "\n";
				sendLogonFail(peer, "`4Sorry, but`w " + ((PlayerInfo*)(peer->data))->tankIDName + "`4 account is `5suspended`4! If you have any questions, please contact `5support@growtopiagame.com");
				enet_peer_disconnect_later(peer, 0);
			}*/
						int logStatus = PlayerDB::playerLogin(peer, ((PlayerInfo*)(event.peer->data))->rawName, ((PlayerInfo*)(event.peer->data))->tankIDPass, ((PlayerInfo*)(event.peer->data))->isSuspended);
						
						if (logStatus == 1) {
							((PlayerInfo*)(event.peer->data))->isSuspended;
							time_t now = time(0);
							tm *ltm = localtime(&now);
							//packet::consolemessage(peer, "`rYou have successfully logged into your account!``");
							ENetPeer * currentPeer;
							int count = 0;
						

							string name = "";
							for (currentPeer = server->peers;
								currentPeer < &server->peers[server->peerCount];
								++currentPeer)
							{
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
									continue;
								count++;
								//Going to account for the current player
								count - 1;
							}

							
							if (((PlayerInfo*)(event.peer->data))->tankIDName == "matty") {
								((PlayerInfo*)(event.peer->data))->displayName = "`6@Matty";
							}
							if (((PlayerInfo*)(peer->data))->adminLevel == 500) {
								((PlayerInfo*)(event.peer->data))->displayName = "`#@" + ((PlayerInfo*)(event.peer->data))->tankIDName;
								packet::consolemessage(peer, "`oWelcome back,`w " + ((PlayerInfo*)(event.peer->data))->displayName + "`o. `w" + std::to_string(count) + "`o others are online.");
							}
							else {
								((PlayerInfo*)(event.peer->data))->displayName = ((PlayerInfo*)(event.peer->data))->tankIDName;
								packet::consolemessage(peer, "`oWelcome back,`w " + ((PlayerInfo*)(event.peer->data))->displayName + "`o. `w" + std::to_string(count) + "`o others are online.");
							}
							gamepacket_t p;
							p.Insert("OnSetBux");
							p.Insert(((PlayerInfo*)(event.peer->data))->gems);
							p.Insert(1);
							p.Insert(1);
							p.Insert(1966, 1, 0);
							p.CreatePacket(peer);
							

						}
						else if (logStatus = 2) {
							//packet::consolemessage(peer, "`rWrong username or password!``");
							/*sendLogonFail(peer, "`rWrong username or password!!``");
							enet_peer_disconnect_later(peer, 0);*/
							passBad(peer);
						}
						else if (logStatus = 3) {
							//ENetPeer * peer;
								gamepacket_t p3;
								p3.Insert("OnConsoleMessage");
								p3.Insert("`4Sorry this account (`5" + ((PlayerInfo*)(peer->data))->rawName + "`4) has been suspended. Contact `5https://support.ubi.com/en-GB/games/5847 `4if you have any questions.");
								p3.CreatePacket(peer);
								//enet_peer_disconnect_later(peer, 0);
								enet_peer_disconnect_later(peer, 0);
								cout << "Account: " << ((PlayerInfo*)(peer->data))->rawName << " is suspended and is trying to enter the game";

						}
#else
						
						((PlayerInfo*)(event.peer->data))->displayName = PlayerDB::fixColors(((PlayerInfo*)(event.peer->data))->tankIDName.substr(0, ((PlayerInfo*)(event.peer->data))->tankIDName.length()>18 ? 18 : ((PlayerInfo*)(event.peer->data))->tankIDName.length()));
						if (((PlayerInfo*)(event.peer->data))->displayName.length() < 3) ((PlayerInfo*)(event.peer->data))->displayName = "Person that doesn't know how the name looks!";
#endif
					}
					for (char c : ((PlayerInfo*)(event.peer->data))->displayName) if (c < 0x20 || c>0x7A) ((PlayerInfo*)(event.peer->data))->displayName = "Bad characters in name, remove them!";
					
					if (((PlayerInfo*)(event.peer->data))->country.length() > 4)
					{
						((PlayerInfo*)(event.peer->data))->country = "us";
					}

					//if (getAdminLevel(((PlayerInfo*)(event.peer->data))->rawName, ((PlayerInfo*)(event.peer->data))->tankIDPass) > 998)
					//{
					//	((PlayerInfo*)(event.peer->data))->country = "\interface\rtsoft_logo";
					//}
					if (((PlayerInfo*)(peer->data))->hasLegenName == true) {
						((PlayerInfo*)(peer->data))->displayName = "`9" + ((PlayerInfo*)(peer->data))->rawName + " of Legend";
					}
					if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 999 || ((PlayerInfo*)(peer->data))->adminLevel == 999) {
						sendDeveloperLogged(peer);
					} 

					if (((PlayerInfo*)(peer->data))->rawName == "matty") {
						
						bool haveSuperSupporterName = true;
						
						((PlayerInfo*)(event.peer->data))->displayName = "`6@Matty";
						((PlayerInfo*)(event.peer->data))->country = "rt";
						((PlayerInfo*)(event.peer->data))->haveSuperSupporterName = true;
					}
					if (((PlayerInfo*)(peer->data))->rawName == "seth") {
						bool haveSuperSupporterName = true;
						time_t now = time(0);
						tm *ltm = localtime(&now);
						((PlayerInfo*)(event.peer->data))->displayName = "`6@Seth";
						((PlayerInfo*)(event.peer->data))->country = "rt";
						((PlayerInfo*)(event.peer->data))->haveSuperSupporterName = true;

					}
					if (((PlayerInfo*)(peer->data))->rawName == "hamumu") {
						time_t now = time(0);
						tm *ltm = localtime(&now);
						((PlayerInfo*)(event.peer->data))->displayName = "`6@Hamumu";
						((PlayerInfo*)(event.peer->data))->country = "ha";
						((PlayerInfo*)(event.peer->data))->haveSuperSupporterName = true;
					}
					if (((PlayerInfo*)(peer->data))->rawName == "jenuine") {
						((PlayerInfo*)(event.peer->data))->displayName = "`#@Jenuine";
						((PlayerInfo*)(event.peer->data))->haveSuperSupporterName = true;

					}
					if (((PlayerInfo*)(peer->data))->rawName == "pixelmorph") {
						((PlayerInfo*)(event.peer->data))->displayName = "`#@PixelMorph";
						((PlayerInfo*)(event.peer->data))->haveSuperSupporterName = true;

					}
					if (((PlayerInfo*)(peer->data))->rawName == "yomattty") {
						((PlayerInfo*)(event.peer->data))->displayName = "`#@YoMattty";
						((PlayerInfo*)(event.peer->data))->haveSuperSupporterName = true;

					}
					if (((PlayerInfo*)(peer->data))->rawName == "ubidev") {
						((PlayerInfo*)(event.peer->data))->displayName = "`6@Ubidev";
						((PlayerInfo*)(event.peer->data))->haveSuperSupporterName = true;
					}
					
					if (((PlayerInfo*)(peer->data))->rawName == "matty") {
						//((PlayerInfo*)(event.peer->data))->country = "/interface/flags/ha.rttex|maxLevel";
						((PlayerInfo*)(event.peer->data))->country = "rt";
					}
					if (((PlayerInfo*)(peer->data))->rawName == "yomattty") {
						((PlayerInfo*)(event.peer->data))->country = "au";
					}

					//if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 999)
					if (((PlayerInfo*)(peer->data))->level == 125){
					((PlayerInfo*)(event.peer->data))->country = "us|maxLevel";
					}

					gamepacket_t p2;
					p2.Insert("SetHasGrowID");
					p2.Insert(((PlayerInfo*)(event.peer->data))->haveGrowId);
					p2.Insert(((PlayerInfo*)(peer->data))->tankIDName);
					p2.Insert(((PlayerInfo*)(peer->data))->tankIDPass);
					p2.CreatePacket(peer);
					gamepacket_t p74;
					p74.Insert("OnSetBux");
					p74.Insert(((PlayerInfo*)(peer->data))->gems);
					p74.CreatePacket(peer);
				}
				string pStr = GetTextPointerFromPacket(event.packet);
				//if (strcmp(GetTextPointerFromPacket(event.packet), "action|enter_game\n") == 0 && !((PlayerInfo*)(event.peer->data))->isIn)
				if(pStr.substr(0, 17) == "action|enter_game" && !((PlayerInfo*)(event.peer->data))->isIn)
				{
#ifdef TOTAL_LOG
					cout << "And we are in!" << endl;
#endif
					
					ENetPeer* currentPeer;
					((PlayerInfo*)(event.peer->data))->isIn = true;
					time_t now = time(0);
					tm *ltm = localtime(&now);

					/*for (currentPeer = server->peers;
						currentPeer < &server->peers[server->peerCount];
						++currentPeer)
					{
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
							continue;

						GamePacket p = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "Player `o" + ((PlayerInfo*)(event.peer->data))->tankIDName + "`o just entered the game..."));
						ENetPacket * packet = enet_packet_create(p.data,
							p.len,
							ENET_PACKET_FLAG_RELIABLE);
						enet_peer_send(currentPeer, 0, packet);
						
						enet_host_flush(server);
						delete p.data;
					}*/
					sendWorldOffers(peer);
					
					// growmoji
					gamepacket_t  p;
					p.Insert("OnEmoticonDataChanged");
					p.Insert(201560520);
					p.Insert("(wl)|Ä|1&(yes)|Ä‚|1&(no)|Äƒ|1&(love)|Ä„|1&(oops)|Ä…|1&(shy)|Ä†|1&(wink)|Ä‡|1&(tongue)|Äˆ|1&(agree)|Ä‰|1&(sleep)|ÄŠ|1&(punch)|Ä‹|1&(music)|ÄŒ|1&(build)|Ä|1&(megaphone)|ÄŽ|1&(sigh)|Ä|1&(mad)|Ä|1&(wow)|Ä‘|1&(dance)|Ä’|1&(see-no-evil)|Ä“|1&(bheart)|Ä”|1&(heart)|Ä•|1&(grow)|Ä–|1&(gems)|Ä—|1&(kiss)|Ä˜|1&(gtoken)|Ä™|1&(lol)|Äš|1&(smile)|Ä€|1&(cool)|Äœ|1&(cry)|Ä|1&(vend)|Äž|1&(bunny)|Ä›|1&(cactus)|ÄŸ|1&(pine)|Ä¤|1&(peace)|Ä£|1&(terror)|Ä¡|1&(troll)|Ä¢|1&(evil)|Ä¢|1&(fireworks)|Ä¦|1&(football)|Ä¥|1&(alien)|Ä§|1&(party)|Ä¨|1&(pizza)|Ä©|1&(clap)|Äª|1&(song)|Ä«|1&(ghost)|Ä¬|1&(nuke)|Ä­|1&(halo)|Ä®|1&(turkey)|Ä¯|1&(gift)|Ä°|1&(cake)|Ä±|1&(heartarrow)|Ä²|1&(lucky)|Ä³|1&(shamrock)|Ä´|1&(grin)|Äµ|1&(ill)|Ä¶|1&");
					p.CreatePacket(peer);
					
					
					packet::consolemessage(peer, "Server made by Growtopia Noobs, some fixes by iProgramInCpp and items from Nenkai.");
					if (serverEvent == 1) {
						packet::consolemessage(peer, "`2Growtopia is a year older! `oGet `480%`o more Gems for your money this week, and enjoy special party events!");
					}
					else if (serverEvent == 2) {
						packet::consolemessage(peer, "`2Happy Lunar New Year!`o It's the Year of the Ox! Open those fortune cookies and ");
					}
					else if (serverEvent == 3) {
						packet::consolemessage(peer, "`2It's Valentine's Week!`o Spend the time enjoying the company of your friends and sharing the love and peace of the universe, man!");
					}
					else if (serverEvent == 4) {
						packet::consolemessage(peer, "`2It's St Patricks Week!`o");
					}
					else if (serverEvent == 5) {
						packet::consolemessage(peer, "`2It's Easter Week!`o");

					}
					else if (serverEvent == 6) {
						packet::consolemessage(peer, "`2It's Cinco De Mayo Week!`o");

					}
					else if (serverEvent == 7) {
						packet::consolemessage(peer, "`2It's Super Pineapple Week!`o");

					}
					else if (serverEvent == 8) {
						packet::consolemessage(peer, "`2IT'S SUMMERFEST! `oGrab your Water Wings and jump in the ocean! Have a barbecue and build a Sand Castle.");

					}
					else if (serverEvent == 9) {
						packet::consolemessage(peer, "`2Player Appreciation Week!`o Seven days of festivities and random new stuff suggested by players!");

					}
					else if (serverEvent == 10) {
						packet::consolemessage(peer, "`2HARVEST FESTIVAL! `oCelebrate harvest festivals from around the world with special events and items.Every time you harvest a tree, there is a small chance a Mooncake will pop out!");
					}
					else if (serverEvent == 11) {
						packet::consolemessage(peer, "`2Wing Week!`o Spread your wings and welcome the new annual event, Wing Week is here!");
					}
					else if (serverEvent == 12) {
						packet::consolemessage(peer, "`2It's Halloween!`o Put on a costume and knock on some doors, or if you're feeling bold, visit GROWGANOTH for a taste of pure swirl evil.");
					}
					else if (serverEvent == 13) {
						packet::consolemessage(peer, "`2Happy Thanksgiving!`o Thanksgiving Week is here, and turkeytastic treasures await!");
					}
					else if (serverEvent == 14) {
						packet::consolemessage(peer, "`2It's Winterfest!`o Presents will be appearing all over Growtopia - what will you do with yours? Be selfish and open it, or share?");
					}
					else if (serverEvent == 15) {
						packet::consolemessage(peer, "`2The Carnival has come to town! `oVisit CARNIVAL to play games and win unique prizes. But hurry, in a couple days they will move on.");
					}
					else if (serverEvent == 18) {
						packet::consolemessage(peer, "`2The Grand Tournament has come to Growtopia!`o See who's the champion this month in the world TOURNAMENT!");
					}
					else if (serverEvent == 24) {
						packet::consolemessage(peer, "`1It's Apology Week!`o Everybody who plays is`2 Lucky`o (more gems and seeds from smashing blocks), and you get `425%`o more Gems when you buy gems!");
					}
					else if (serverEvent == 25) {
						packet::consolemessage(peer, "`2What's that in the sky??`o A comet is blazing a trail through the night! It will only be here for 24 hours...");
					}
					if (serverEvent == 27) {
						packet::consolemessage(peer, "`4WARNING!`o Server is currently in `4maintenance`o mode! `4NO`o players will be able to enter except developers.");
					}
					else if (serverMaintenance == 0) {
						//do nothing k?
					}
					PlayerInventory inventory;
					for (int i = 0; i < 200; i++)
					{
						InventoryItem it;
						it.itemID = (i * 2) + 2;
						it.itemCount = 200;
						inventory.items.push_back(it);
					}
					((PlayerInfo*)(event.peer->data))->inventory = inventory;

					{
						//GamePacket p = packetEnd(appendString(appendString(createPacket(), "OnDialogRequest"), "set_default_color|`o\n\nadd_label_with_icon|big|`wThe Growtopia Gazette``|left|5016|\n\nadd_spacer|small|\n\nadd_image_button|banner|interface/large/news_banner.rttex|noflags|||\n\nadd_spacer|small|\n\nadd_textbox|`wSeptember 10:`` `5Surgery Stars end!``|left|\n\nadd_spacer|small|\n\n\n\nadd_textbox|Hello Growtopians,|left|\n\nadd_spacer|small|\n\n\n\nadd_textbox|Surgery Stars is over! We hope you enjoyed it and claimed all your well-earned Summer Tokens!|left|\n\nadd_spacer|small|\n\nadd_spacer|small|\n\nadd_textbox|As we announced earlier, this month we are releasing the feature update a bit later, as we're working on something really cool for the monthly update and we're convinced that the wait will be worth it!|left|\n\nadd_spacer|small|\n\nadd_textbox|Check the Forum here for more information!|left|\n\nadd_spacer|small|\n\nadd_url_button|comment|`wSeptember Updates Delay``|noflags|https://www.growtopiagame.com/forums/showthread.php?510657-September-Update-Delay&p=3747656|Open September Update Delay Announcement?|0|0|\n\nadd_spacer|small|\n\nadd_spacer|small|\n\nadd_textbox|Also, we're glad to invite you to take part in our official Growtopia survey!|left|\n\nadd_spacer|small|\n\nadd_url_button|comment|`wTake Survey!``|noflags|https://ubisoft.ca1.qualtrics.com/jfe/form/SV_1UrCEhjMO7TKXpr?GID=26674|Open the browser to take the survey?|0|0|\n\nadd_spacer|small|\n\nadd_textbox|Click on the button above and complete the survey to contribute your opinion to the game and make Growtopia even better! Thanks in advance for taking the time, we're looking forward to reading your feedback!|left|\n\nadd_spacer|small|\n\nadd_spacer|small|\n\nadd_textbox|And for those who missed PAW, we made a special video sneak peek from the latest PAW fashion show, check it out on our official YouTube channel! Yay!|left|\n\nadd_spacer|small|\n\nadd_url_button|comment|`wPAW 2018 Fashion Show``|noflags|https://www.youtube.com/watch?v=5i0IcqwD3MI&feature=youtu.be|Open the Growtopia YouTube channel for videos and tutorials?|0|0|\n\nadd_spacer|small|\n\nadd_textbox|Lastly, check out other September updates:|left|\n\nadd_spacer|small|\n\nadd_label_with_icon|small|IOTM: The Sorcerer's Tunic of Mystery|left|24|\n\nadd_label_with_icon|small|New Legendary Summer Clash Branch|left|24|\n\nadd_spacer|small|\n\nadd_textbox|`$- The Growtopia Team``|left|\n\nadd_spacer|small|\n\nadd_spacer|small|\n\n\n\n\n\nadd_url_button|comment|`wOfficial YouTube Channel``|noflags|https://www.youtube.com/c/GrowtopiaOfficial|Open the Growtopia YouTube channel for videos and tutorials?|0|0|\n\nadd_url_button|comment|`wSeptember's IOTM: `8Sorcerer's Tunic of Mystery!````|noflags|https://www.growtopiagame.com/forums/showthread.php?450065-Item-of-the-Month&p=3392991&viewfull=1#post3392991|Open the Growtopia website to see item of the month info?|0|0|\n\nadd_spacer|small|\n\nadd_label_with_icon|small|`4WARNING:`` `5Drop games/trust tests`` and betting games (like `5Casinos``) are not allowed and will result in a ban!|left|24|\n\nadd_label_with_icon|small|`4WARNING:`` Using any kind of `5hacked client``, `5spamming/text pasting``, or `5bots`` (even with an alt) will likely result in losing `5ALL`` your accounts. Seriously.|left|24|\n\nadd_label_with_icon|small|`4WARNING:`` `5NEVER enter your GT password on a website (fake moderator apps, free gemz, etc) - it doesn't work and you'll lose all your stuff!|left|24|\n\nadd_spacer|small|\n\nadd_url_button|comment|`wGrowtopia on Facebook``|noflags|http://growtopiagame.com/facebook|Open the Growtopia Facebook page in your browser?|0|0|\n\nadd_spacer|small|\n\nadd_button|rules|`wHelp - Rules - Privacy Policy``|noflags|0|0|\n\n\nadd_quick_exit|\n\nadd_spacer|small|\nadd_url_button|comment|`wVisit Growtopia Forums``|noflags|http://www.growtopiagame.com/forums|Visit the Growtopia forums?|0|0|\nadd_spacer|small|\nadd_url_button||`wWOTD: `1THELOSTGOLD`` by `#iWasToD````|NOFLAGS|OPENWORLD|THELOSTGOLD|0|0|\nadd_spacer|small|\nadd_url_button||`wVOTW: `1Yodeling Kid - Growtopia Animation``|NOFLAGS|https://www.youtube.com/watch?v=UMoGmnFvc58|Watch 'Yodeling Kid - Growtopia Animation' by HyerS on YouTube?|0|0|\nend_dialog|gazette||OK|"));
						packet::dialog(peer, newslist);
					}
					/*if (((PlayerInfo*)(event.peer->data))->haveGrowId == false) {
						string worldname = "START";
						sendPlayerToWorld(peer, (PlayerInfo*)(peer->data), worldname);
						WorldInfo info = worldDB.get(worldname);
						sendWorld(peer, &info);
						/*string worldname = "START";
						WorldInfo info = worldDB.get(worldname);
						sendWorld(peer, &info);
					}
					else {
						//nothing
					}*/
				}
				if (strcmp(GetTextPointerFromPacket(event.packet), "action|refresh_item_data\n") == 0)
				{
					if (itemsDat != NULL) {
						ENetPacket * packet = enet_packet_create(itemsDat,
							itemsDatSize + 60,
							ENET_PACKET_FLAG_RELIABLE);
						enet_peer_send(peer, 0, packet);
						((PlayerInfo*)(peer->data))->isUpdating = true;
						//enet_peer_disconnect_later(peer, 0); // TODO: add this back, and fix it properly
						//enet_host_flush(server);
					}
					// TODO FIX refresh_item_data ^^^^^^^^^^^^^^
				}
				break;
			}
			default:
				cout << "Unknown packet type " << messageType << endl;
				break;
			case 3:
			{
				WorldInfo* worldInfo;
				//cout << GetTextPointerFromPacket(event.packet) << endl;
				std::stringstream ss(GetTextPointerFromPacket(event.packet));
				std::string to;
				bool isJoinReq = false;
				bool isNukedx = false;
			
		
				while (std::getline(ss, to, '\n')) {
					string id = to.substr(0, to.find("|"));
					string act = to.substr(to.find("|") + 1, to.length() - to.find("|") - 1);
					if (id == "name" && isJoinReq)
					{
#ifdef TOTAL_LOG
						cout << "Entering some world..." << endl;
#endif
						if (!((PlayerInfo*)(peer->data))->hasLogon) break;
						
						try {
							WorldInfo info = worldDB.get(act);
							for (int i = 0; i < nukedworlds.size(); i++)
							{
								if (act == nukedworlds[i])
									isNukedx = true;
								else
									isNukedx = false;
							}
							if (act.length() > 30) {
								packet::consolemessage(peer, "`4Sorry, but world names with more than 30 characters are not allowed!");
								((PlayerInfo*)(peer->data))->currentWorld = "EXIT";
								gamepacket_t p;
								p.Insert("OnFailedToEnterWorld");
								p.Insert(1);
								p.CreatePacket(peer);

							} 
							if (!isNukedx){
								WorldInfo info = worldDB.get(act);
								ENetPeer * currentPeer;
								
								string worldNames = ((PlayerInfo*)(peer->data))->currentWorld;
								sendWorld(peer, &info);

								int x = 3040;
								int y = 736;
								int weather = 7;
								for (int j = 0; j < info.width*info.height; j++)
								{
									if (info.items[j].foreground == 6) {
										x = (j%info.width) * 32;
										y = (j / info.width) * 32;
									}
								}
								if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 499 || ((PlayerInfo*)(peer->data))->adminLevel > 499) {
									if (((PlayerInfo*)(peer->data))->isInvisible == true) {
										packet::onspawn(peer, "spawn|avatar\nnetID|" + std::to_string(cId) + "\nuserID|" + std::to_string(cId) + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(x) + "|" + std::to_string(y) + "\nname|``" + ((PlayerInfo*)(event.peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(event.peer->data))->country + "\ninvis|1\nmstate|1\nsmstate|1\ntype|local\n");
									}
									else {
										packet::onspawn(peer, "spawn|avatar\nnetID|" + std::to_string(cId) + "\nuserID|" + std::to_string(cId) + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(x) + "|" + std::to_string(y) + "\nname|``" + ((PlayerInfo*)(event.peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(event.peer->data))->country + "\ninvis|0\nmstate|1\nsmstate|1\ntype|local\n");
									}
								}
								else {
									packet::onspawn(peer, "spawn|avatar\nnetID|" + std::to_string(cId) + "\nuserID|" + std::to_string(cId) + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(x) + "|" + std::to_string(y) + "\nname|``" + ((PlayerInfo*)(event.peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(event.peer->data))->country + "\ninvis|0\nmstate|0\nsmstate|0\ntype|local\n");
								}
								((PlayerInfo*)(event.peer->data))->netID = cId;
								onPeerConnect(peer);
								cId++;
								sendState(peer);
						
								sendClothes(peer);
								

								sendInventory(peer, ((PlayerInfo*)(event.peer->data))->inventory);
								/*gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("`oTesting so i can add a message to enter worlds. I have no idea tb");
								p.CreatePacket(peer);*/
								string text = "action|play_sfx\nfile|audio/door_open.wav\ndelayMS|0\n";
								BYTE* data = new BYTE[5 + text.length()];
								BYTE zero = 0;
								int type = 3;
								memcpy(data, &type, 4);
								memcpy(data + 4, text.c_str(), text.length());
								memcpy(data + 4 + text.length(), &zero, 1);
								
								for (currentPeer = server->peers;
									currentPeer < &server->peers[server->peerCount];
									++currentPeer)
								{
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
										continue;
									if (isHere(peer, currentPeer))
									{
										ENetPacket * packet2 = enet_packet_create(data,
											5 + text.length(),
											ENET_PACKET_FLAG_RELIABLE);

										enet_peer_send(currentPeer, 0, packet2);
										sendClothes(peer);
										sendState(peer);
										sendClothes(currentPeer);
										sendState(currentPeer);
									}
								}
								/*if (std::find(nukedworlds.begin(), nukedworlds.end(), worldNames) != nukedworlds.end()) {
									gamepacket_t p5;
									p5.Insert("OnConsoleMessage");
									p5.Insert("`4This world is nuked and only accessible by moderators.");
									p5.CreatePacket(peer);
								}*/
								PlayerInventory inventory = ((PlayerInfo *)(peer->data))->inventory;
								InventoryItem item1, item2, item3;
								int itemPunch = 18;
								int itemWrench = 32;
								int itemMainDoor = 2;
								
								item1.itemID = itemPunch;
								item2.itemID = itemWrench;
								item3.itemID = itemMainDoor;
								item1.itemCount = 1;
								item2.itemCount = 1;
								item3.itemCount = 200;

								inventory.items.push_back(item1);
								inventory.items.push_back(item2);
								inventory.items.push_back(item3);
								sendInventory(peer, inventory);
                                 if (((PlayerInfo*)(peer->data))->taped) {
									 ((PlayerInfo*)(peer->data))->isDuctaped = true;
									 sendState(peer);
								 }
								 else if (!((PlayerInfo*)(peer->data))->taped) {
									 sendState(peer);
								 }
								 sendState(peer);
							}
							/*else if (info.name == "HI") {

                            }*/
							else if (isNukedx && getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) < 900 || ((PlayerInfo*)(peer->data))->adminLevel < 900){
									((PlayerInfo*)(peer->data))->currentWorld = "EXIT";
									gamepacket_t p1;
									p1.Insert("OnConsoleMessage");
									p1.Insert("`oThat world is inaccessible.");
									p1.CreatePacket(peer);
									gamepacket_t p;
									p.Insert("OnFailedToEnterWorld");
									p.Insert(1);
									p.CreatePacket(peer);
									cout << "A user tried to enter a nuked world??\n";

								
							}else if (isNukedx && getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) == 999 || ((PlayerInfo*)(peer->data))->adminLevel == 999){
								WorldInfo info = worldDB.get(act);
								sendWorld(peer, &info);
								int x = 3040;
								int y = 736;
								
								for (int j = 0; j < info.width*info.height; j++)
								{
									if (info.items[j].foreground == 6) {
										x = (j%info.width) * 32;
										y = (j / info.width) * 32;
									}
								}
								if (getAdminLevel(((PlayerInfo*)(peer->data))->rawName, ((PlayerInfo*)(peer->data))->tankIDPass) > 500 || ((PlayerInfo*)(peer->data))->adminLevel > 500) {
									if (((PlayerInfo*)(peer->data))->isInvisible == true) {
										packet::onspawn(peer, "spawn|avatar\nnetID|" + std::to_string(cId) + "\nuserID|" + std::to_string(cId) + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(x) + "|" + std::to_string(y) + "\nname|``" + ((PlayerInfo*)(event.peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(event.peer->data))->country + "\ninvis|1\nmstate|1\nsmstate|1\ntype|local\n");
									}
									else {
										packet::onspawn(peer, "spawn|avatar\nnetID|" + std::to_string(cId) + "\nuserID|" + std::to_string(cId) + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(x) + "|" + std::to_string(y) + "\nname|``" + ((PlayerInfo*)(event.peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(event.peer->data))->country + "\ninvis|0\nmstate|1\nsmstate|1\ntype|local\n");
									}
								}
								else {
									packet::onspawn(peer, "spawn|avatar\nnetID|" + std::to_string(cId) + "\nuserID|" + std::to_string(cId) + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(x) + "|" + std::to_string(y) + "\nname|``" + ((PlayerInfo*)(event.peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(event.peer->data))->country + "\ninvis|0\nmstate|0\nsmstate|0\ntype|local\n");
								}
								((PlayerInfo*)(event.peer->data))->netID = cId;
								onPeerConnect(peer);
								cId++;

								sendInventory(peer, ((PlayerInfo*)(event.peer->data))->inventory);
								sendState(peer);
							
								sendClothes(peer);
							

								PlayerInventory inventory = ((PlayerInfo *)(peer->data))->inventory;
								InventoryItem item1, item2, item3;
								int itemPunch = 18;
								int itemWrench = 32;
								int itemMainDoor = 6;

								item1.itemID = itemPunch;
								item2.itemID = itemWrench;
								item3.itemID = itemMainDoor;
								item1.itemCount = 1;
								item2.itemCount = 1;
								item3.itemCount = 200;

								inventory.items.push_back(item1);
								inventory.items.push_back(item2);
								inventory.items.push_back(item3);
								sendInventory(peer, inventory);
								if (((PlayerInfo*)(peer->data))->taped) {
									((PlayerInfo*)(peer->data))->isDuctaped = true;
									sendState(peer);
								}
								else if (!((PlayerInfo*)(peer->data))->taped) {
									sendState(peer);
								}
								sendState(peer);
							}
						}
						catch (int e) {
							if (e == 1) {
								((PlayerInfo*)(peer->data))->currentWorld = "EXIT";
								gamepacket_t p;
								p.Insert("OnFailedToEnterWorld");
								p.Insert(1);
								p.CreatePacket(peer);
								packet::consolemessage(peer, "You have exited the world.");
							}
							else if (e == 2) {
								((PlayerInfo*)(peer->data))->currentWorld = "EXIT";
								gamepacket_t p;
								p.Insert("OnFailedToEnterWorld");
								p.Insert(1);
								p.CreatePacket(peer);
								packet::consolemessage(peer, "You have entered bad characters in the world name!");
							}
							else if (e == 3) {
								((PlayerInfo*)(peer->data))->currentWorld = "EXIT";
								gamepacket_t p;
								p.Insert("OnFailedToEnterWorld");
								p.Insert(1);
								p.CreatePacket(peer);
								packet::consolemessage(peer, "Exit from what? Click back if you're done playing.");
							}
							else {
								((PlayerInfo*)(peer->data))->currentWorld = "EXIT";
								gamepacket_t p;
								p.Insert("OnFailedToEnterWorld");
								p.Insert(1);
								p.CreatePacket(peer);
								packet::consolemessage(peer, "I know this menu is magical and all, but it has its limitations! You can't visit this world!");
							}
						}
					}
						if (id == "action")
						{

							if (act == "join_request")
							{
								isJoinReq = true;
							}
							if (act == "quit_to_exit")
							{
								sendPlayerLeave(peer, (PlayerInfo*)(event.peer->data));
								((PlayerInfo*)(peer->data))->currentWorld = "EXIT";
								sendWorldOffers(peer);
								sendSaveSet(peer);

							}
							if (act == "quit")
							{
								enet_peer_disconnect_later(peer, 0);
							}
						}
					}
					break;
			}
			case 4:
			{
				{
					BYTE* tankUpdatePacket = GetStructPointerFromTankPacket(event.packet); 
					
					if (tankUpdatePacket)
					{
						PlayerMoving* pMov = unpackPlayerMoving(tankUpdatePacket);
						if (((PlayerInfo*)(event.peer->data))->isGhost) {
							((PlayerInfo*)(event.peer->data))->isInvisible = true;
							((PlayerInfo*)(event.peer->data))->x1 = pMov->x;
							((PlayerInfo*)(event.peer->data))->y1 = pMov->y;
							pMov->x = -1000000;
							pMov->y = -1000000;
						}
						
						switch (pMov->packetType)
						{
						case 0:
							((PlayerInfo*)(event.peer->data))->x = pMov->x;
							((PlayerInfo*)(event.peer->data))->y = pMov->y;
							((PlayerInfo*)(event.peer->data))->isRotatedLeft = pMov->characterState & 0x10;
							sendPData(peer, pMov);
							if (!((PlayerInfo*)(peer->data))->joinClothesUpdated)
							{
								((PlayerInfo*)(peer->data))->joinClothesUpdated = true;
								updateAllClothes(peer);
							}
							break;

						default:
							break;
						}
						PlayerMoving *data2 = unpackPlayerMoving(tankUpdatePacket);
						//cout << data2->packetType << endl;
						if (data2->packetType == 11)
						{
							//sendCollect(peer, ((PlayerInfo*)(peer->data))->netID, data2->plantingTree);
							sendCollect(peer, ((PlayerInfo*)(event.peer->data))->netID, ((PlayerInfo*)(event.peer->data))->x, ((PlayerInfo*)(event.peer->data))->y, data2->plantingTree);
							if (data2->plantingTree == 112) {
								cout << "gems\n";
							}
						}
						if (data2->packetType == 7)
						{
							if(data2->punchX < world->width && data2->punchY < world->height)
							if (getItemDef(world->items[data2->punchX + (data2->punchY * world->width)].foreground).blockType == BlockTypes::MAIN_DOOR) {
									sendPlayerLeave(peer, (PlayerInfo*)(event.peer->data));
									((PlayerInfo*)(peer->data))->currentWorld = "EXIT";
									sendWorldOffers(peer);

								}
						}
						if (data2->packetType == 10)
						{
							//cout << pMov->x << ";" << pMov->y << ";" << pMov->plantingTree << ";" << pMov->punchX << ";" << pMov->punchY << ";" << pMov->characterState << endl;
							ItemDefinition def;
							try {
								def = getItemDef(pMov->plantingTree);
							}
							catch (int e) {
								goto END_CLOTHSETTER_FORCE;
							}
							
							switch (def.clothType) {
							case 0:
								if (((PlayerInfo*)(event.peer->data))->cloth0 == pMov->plantingTree)
								{
									((PlayerInfo*)(event.peer->data))->cloth0 = 0;
									break;
								}
								((PlayerInfo*)(event.peer->data))->cloth0 = pMov->plantingTree;
								break;
							case 1:
								if (((PlayerInfo*)(event.peer->data))->cloth1 == pMov->plantingTree)
								{
									((PlayerInfo*)(event.peer->data))->cloth1 = 0;
									sendState(peer);
									break;
								} 
								{
									int item = pMov->plantingTree;
									if (item == 1780) {
										if (((PlayerInfo*)(peer->data))->hasLegenBot == false) {
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("`oYou aren't legendary enough to wear that!");
											p.CreatePacket(peer);
											((PlayerInfo*)(event.peer->data))->cloth1 = 0;
										}
										else {
											((PlayerInfo*)(event.peer->data))->cloth1 = pMov->plantingTree;
										}
									}
									else {
										((PlayerInfo*)(event.peer->data))->cloth1 = pMov->plantingTree;
									}
								}
								break;
							case 2:
								if (((PlayerInfo*)(event.peer->data))->cloth2 == pMov->plantingTree)
								{
									((PlayerInfo*)(event.peer->data))->cloth2 = 0;
									break;
								}
								((PlayerInfo*)(event.peer->data))->cloth2 = pMov->plantingTree;
								break;
							case 3:
								if (((PlayerInfo*)(event.peer->data))->cloth3 == pMov->plantingTree)
								{
									((PlayerInfo*)(event.peer->data))->cloth3 = 0;
									break;
								}
								((PlayerInfo*)(event.peer->data))->cloth3 = pMov->plantingTree;
								break;
							case 4:
								if (((PlayerInfo*)(event.peer->data))->cloth4 == pMov->plantingTree)
								{
									((PlayerInfo*)(event.peer->data))->cloth4 = 0;
									break;
								}
								((PlayerInfo*)(event.peer->data))->cloth4 = pMov->plantingTree;
								break;
							case 5:
								if (((PlayerInfo*)(event.peer->data))->cloth5 == pMov->plantingTree)
								{
									((PlayerInfo*)(event.peer->data))->cloth5 = 0;
									sendState(peer);
									break;
								}
								{
									int item = pMov->plantingTree;
									if (item == 2592) {
										if (((PlayerInfo*)(peer->data))->hasLegenKatana == false) {
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("`oYou aren't legendary enough to wear that!");
											p.CreatePacket(peer);
											((PlayerInfo*)(event.peer->data))->cloth5 = 0;
										}
										else {
											((PlayerInfo*)(event.peer->data))->cloth5 = pMov->plantingTree;
										}
										

									}else if (item == 1782) {
										if (((PlayerInfo*)(peer->data))->hasLegenDrag == false) {
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("`oYou aren't legendary enough to wear that!");
											p.CreatePacket(peer);
											((PlayerInfo*)(event.peer->data))->cloth5 = 0;
										}
										else {
											((PlayerInfo*)(event.peer->data))->cloth5 = pMov->plantingTree;
										}
									}
									else if (item == 6026) {
										if (((PlayerInfo*)(peer->data))->hasLegenWhip == false) {
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("`oYou aren't legendary enough to wear that!");
											p.CreatePacket(peer);
											((PlayerInfo*)(event.peer->data))->cloth5 = 0;
										}
										else {
											((PlayerInfo*)(event.peer->data))->cloth5 = pMov->plantingTree;
										}
									}
									else {
										((PlayerInfo*)(event.peer->data))->cloth5 = pMov->plantingTree;
									}
								}
								break;
								//hands
							case 6:
								if (((PlayerInfo*)(event.peer->data))->cloth6 == pMov->plantingTree)
								{
									((PlayerInfo*)(event.peer->data))->cloth6 = 0;
									((PlayerInfo*)(event.peer->data))->canDoubleJump = false;
									sendState(peer);
									break;
								}
								{
									((PlayerInfo*)(event.peer->data))->cloth6 = pMov->plantingTree;
									int item = pMov->plantingTree;
									if (item == 156 || item == 362 || item == 678 || item == 736 || item == 818 || item == 1206 || item == 1460 || item == 1550 || item == 1574 || item == 1668 || item == 1672 || item == 1674 || item == 1784 || item == 1824 || item == 1936 || item == 1938 || item == 1970 || item == 2254 || item == 2256 || item == 2258 || item == 2260 || item == 2262 || item == 2264 || item == 2390 || item == 2392 || item == 3120 || item == 3308 || item == 3512 || item == 4534 || item == 4986 || item == 5754 || item == 6144 || item == 6334 || item == 6694 || item == 6818 || item == 6842 || item == 1934 || item == 3134 || item == 6004 || item == 1780 || item == 2158 || item == 2160 || item == 2162 || item == 2164 || item == 2166 || item == 2168 || item == 2438 || item == 2538 || item == 2778 || item == 3858 || item == 350 || item == 998 || item == 1738 || item == 2642 || item == 2982 || item == 3104 || item == 3144 || item == 5738 || item == 3112 || item == 2722 || item == 3114 || item == 4970 || item == 4972 || item == 5020 || item == 6284 || item == 4184 || item == 4628 || item == 5322 || item == 4112 || item == 4114 || item == 3442) {
										((PlayerInfo*)(event.peer->data))->canDoubleJump = true;
									}
									else {
										((PlayerInfo*)(event.peer->data))->canDoubleJump = false;
									}
									// ^^^^ wings
									sendState(peer);
								}
								break;
							case 7:
								if (((PlayerInfo*)(event.peer->data))->cloth7 == pMov->plantingTree)
								{
									((PlayerInfo*)(event.peer->data))->cloth7 = 0;
									break;
								}
								((PlayerInfo*)(event.peer->data))->cloth7 = pMov->plantingTree;
								break;
							case 8:
								if (((PlayerInfo*)(event.peer->data))->cloth8 == pMov->plantingTree)
								{
									((PlayerInfo*)(event.peer->data))->cloth8 = 0;
									break;
								}
								((PlayerInfo*)(event.peer->data))->cloth8 = pMov->plantingTree;
								break;
							case 9:
								if (((PlayerInfo*)(event.peer->data))->cloth9 == pMov->plantingTree)
								{
									((PlayerInfo*)(event.peer->data))->cloth9 = 0;
									break;
								}
								((PlayerInfo*)(event.peer->data))->cloth9 = pMov->plantingTree;
								break;							
							default:
#ifdef TOTAL_LOG
								cout << "Invalid item activated: " << pMov->plantingTree << " by " << ((PlayerInfo*)(event.peer->data))->displayName << endl;
#endif
								break;
							}
							sendClothes(peer);
							// activate item
						END_CLOTHSETTER_FORCE:;
						}
						if (data2->packetType == 18)
						{
							sendPData(peer, pMov);
							// add talk buble
						}
						if (data2->packetType == 3) {
							cout << "did something\n";
							if (data2->plantingTree == 3) {
								string x = "eventType|0\neventName|313_SEED\nDevice_Id|^^\nGrow_Id|0\nSeed_id|3\nSeed_rarity|1\nTime_passed|0\nTime_to_grow|31\nWorld_name|^^";
								sendData(peer, 6, (char*)x.c_str(), x.length());
								cout << "\n" << x << endl;
							}

						}
						if (data2->punchX != -1 && data2->punchY != -1) {
							//cout << data2->packetType << endl;
							if (data2->packetType == 3)
							{
								sendTileUpdate(data2->punchX, data2->punchY, data2->plantingTree, ((PlayerInfo*)(event.peer->data))->netID, peer);
							}
							else {

							}
							
							/*PlayerMoving data;
							//data.packetType = 0x14;
							data.packetType = 0x3;
							//data.characterState = 0x924; // animation
							data.characterState = 0x0; // animation
							data.x = data2->punchX;
							data.y = data2->punchY;
							data.punchX = data2->punchX;
							data.punchY = data2->punchY;
							data.XSpeed = 0;
							data.YSpeed = 0;
							data.netID = ((PlayerInfo*)(event.peer->data))->netID;
							data.plantingTree = data2->plantingTree;
							SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
							cout << "Tile update at: " << data2->punchX << "x" << data2->punchY << endl;*/
							
						}
						delete data2;
						delete pMov;
					}

					else {
						cout << "Got bad tank packet";
					}
					/*char buffer[2048];
					for (int i = 0; i < event->packet->dataLength; i++)
					{
					sprintf(&buffer[2 * i], "%02X", event->packet->data[i]);
					}
					cout << buffer;*/
				}
			}
			break;
			case 5:
				break;
			case 6:
				//cout << GetTextPointerFromPacket(event.packet) << endl;
				break;
			}
			enet_packet_destroy(event.packet);
			break;
		}
		case ENET_EVENT_TYPE_DISCONNECT:
#ifdef TOTAL_LOG
			printf("Peer disconnected.\n");
#endif
			/* Reset the peer's client information. */
			/*ENetPeer* currentPeer;
			for (currentPeer = server->peers;
				currentPeer < &server->peers[server->peerCount];
				++currentPeer)
			{
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
					continue;

				GamePacket p = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "Player `o" + ((PlayerInfo*)(event.peer->data))->tankIDName + "`o just left the game..."));
				ENetPacket * packet = enet_packet_create(p.data,
					p.len,
					ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(currentPeer, 0, packet);
				enet_host_flush(server);
			}*/
			sendPlayerLeave(peer, (PlayerInfo*)(event.peer->data));
			((PlayerInfo*)(event.peer->data))->inventory.items.clear();
			delete (PlayerInfo*)event.peer->data;
			event.peer->data = NULL;
		}
	}
	cout << "Program ended??? Huh?" << endl;
	while (1);
	return 0;
}
