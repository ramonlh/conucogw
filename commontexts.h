//P(htmlHead_i0) = "<!DOCTYPE html><html><head><title>";
P(htmlHead_i0) = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" "
                 "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">"
                 "<html xmlns=\"http://www.w3.org/1999/xhtml\"><head><title>";
P(htmlHead_ix) = "Conuco Web server";
P(htmlHead_i1) = "</title>"
    "<meta name=\"viewport\" content=\"width=device-width; initial-scale=2.5\"/>"
    "<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\"/>"
    
    "<style type=\"text/css\">"
    
    "table.tmenu {font-family:Verdana; font-size:11pt;color:#333333;border-width:2px;border-color:#666666;"
                 "border-collapse:collapse; background-color:#dedede;}"     
    "table.tmenu th {color:#000000;border-width:2px;padding:8px;border-style:solid;border-color:#666666;"
                    "background-color:#FDFF9A;}"
    "table.tmenu td {color:#FFFFFF;border-width:2px;padding:8px;border-style:solid;border-color:#666666;"
                    "background-color:#dedede;}"

    "table.tpanel {font-family:Verdana; font-size:9pt;border-color:#666666;border-width:2px;"
                    "border-collapse:collapse;color:#FFFFFF;background-color:#515772;text-align:left;}"      
    "table.tpanel th {border-color:#666666;border-width:2px;padding:8px;border-style:solid;color:#000000; background-color:#FFFF00;text-align:center;}"
    "table.tpanel td {border-color:#666666;border-width:2px;padding:8px; border-style:solid; background-color:#515772;}"

    // usado en tablas de configuración, programación etc CENTRADO
    "table.tconf {text-align:center;font-family:Verdana; font-color:#000000;font-size:9pt; border-color:#666666;border-width:2px;border-collapse:collapse;color:#FFFFFF;background-color:#515772;text-align:left;}"      
    "table.tconf th {color:#000000;border-color:#666666;border-width:2px;padding:4px;border-style:solid;color:#000000; background-color:#FFFF00;text-align:center;}"
    "table.tconf td {text-align:center;color:#000000;border-color:#666666;border-width:2px;padding:4px; border-style:solid; background-color:#DEDEDE;}"

    // usado en tablas de configuración, programación etc NO CENTRADO
    "table.tnormal {font-family:Verdana; font-color:#000000;font-size:9pt; border-color:#666666;border-width:2px;border-collapse:collapse;color:#FFFFFF;background-color:#515772;text-align:left;}"      
    "table.tnormal th {color:#000000;border-color:#666666;border-width:2px;padding:4px;border-style:solid;color:#000000; background-color:#FFFF00;}"
    "table.tnormal td {color:#000000;border-color:#666666;border-width:2px;padding:4px; border-style:solid; background-color:#DEDEDE;}"
    "</style>";

/////////////    CÓDIGO DE SEGUIMIENTO Google Analytics

P(seguimiento)="<script type=\"text/javascript\">"
               "(function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){"
               "(i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),"
               "m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)"
               "})(window,document,'script','//www.google-analytics.com/analytics.js','ga');"
               "ga('create', 'UA-1404365-7', 'zapto.org');"
               "ga('send', 'pageview');"
               "</script>";
//P(seguimiento1)="<script type=\"text/javascript\">"
//               "(function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){"
//               "(i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),"
//               "m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)"
//               "})(window,document,'script','//www.google-analytics.com/analytics.js','ga');"
//               "ga('create', '";
//P(seguimientox)="UA-1404365-7";
//P(seguimiento2)="', 'elconucom.zapto.org');ga('send', 'pageview');</script>";
/////////////

P(head_f) = "</head>";
P(body_i) = "<body>";  P(body_f) = "</body>";
P(html_i) = "<html>";  P(html_f) = "</html>";
P(htmlRefresh_i) = "<meta http-equiv=\"refresh\" content=\"";
P(Http400) = "HTTP 400 - BAD REQUEST";
P(Form_get) = "\" method=\"get\">";
P(Form_post) = "\" method=\"post\">";
P(Form_action) = "<FORM action=\"";
P(value1)="\" value=\"1\"";
P(checked)=" checked";
P(resetNod) = "rn.htm";
P(setupNod2) = "sn2.htm";
P(webclient) = "wc.htm";
P(Form_f) = "</FORM>";
P(Form_input_text_start) = "<input type=\"text\" name=\"";
P(Form_input_text_start_invis) = "<input type=\"text\" style=\"visibility: hidden\" name=\"";
P(Form_input_pass_start) = "<input type=\"password\" name=\"";
P(Form_input_value) = "\" value=\"";
P(Form_input_size) = "\" maxlength=\"1\" size=\"";
P(Form_input_end) = "\">\n";
P(Select_name) = "<SELECT name=\"";
P(Select_name_invis) = "<SELECT style=\"visibility: hidden\" name=\"";
P(Select_f) = "</SELECT>";
P(Max_length) = "\" maxlength=\"";
P(b_i) = "<b>";    P(b_f) = "</b>";
P(h1_i) = "<h1>";  P(h1_f) = "</h1>"; 
P(h2_i) = "<h2>";  P(h2_f) = "</h2>";
P(h3_i) = "<h3>";  P(h3_f) = "</h3>"; 
P(h4_i) = "<h4>";  P(h4_f) = "</h4>";
P(cierre) = ">";
P(comillascierre) = "\"/>";
P(tablamenu) = "<table class=\"tmenu\">";
P(tablapanel) = "<table class=\"tpanel\">";
P(tablaconf) = "<table class=\"tconf\">";
P(tablanormal) = "<table class=\"tnormal\">";
P(br) = "<br />";    P(brn) = "<br />\n";
P(td) = "<td>";  P(td_f) = "</td>";  P(td_if) = "<td></td>";
P(th) = "<th>";  P(th_f) = "</th>";  
P(tr) = "<tr>";  P(tr_f) = "</tr>";  P(tr_if) = "<tr></tr>";
P(trd_i) = "<tr><td>";
P(size_i) = "\" size=\"";
P(trcolor1_i) = "<tr>";
P(trcolor2_i) = "<tr bgcolor=#FFFF00 valign=\"center\">"; // amarillo
P(tdcolor2_i) = "<td bgcolor=#FFFF00 valign=\"center\">"; // amarillo
P(tdcolor2center_i) = "<td bgcolor=#FFFF00 valign=\"center\" align=\"center\">"; // amarillo
P(table_f) = "</table>";
P(colspan) = "<td colspan=\"";
P(rad250kbps) = "250 Kbps";
P(rad1mbps) = "1 Mbps";
P(rad2mbps) = "2 Mbps";
P(mayoroigual) = ">=";          
P(menoroigual) = "<=";          
P(on) = "On";    P(off) = "Off";     P(onoff) = "On/Off";
P(ON) = "ON";    P(OFF) = "OFF";     P(ONOFF) = "ON/OFF";
P(b) = " ";
P(selected) = " selected";
P(optionvalue) = "<OPTION value=\"";
P(option_f) = "</OPTION>";
P(barramayor)="\">";
P(tdbgcolorFFFF00)="<td bgcolor=#FFFF00>";         
P(tdbgcolor8E8E8E)="<td bgcolor=#8E8E8E>";         
P(tdbgcolorAAAAAA)="<td bgcolor=#AAAAAA>";         
P(tdcentercol)= "<td align=\"center\">";
P(tdcentercolspan2)= "<td align=\"center\" colspan=\"2\">";
P(tdcentercolspan3)= "<td align=\"center\" colspan=\"3\">";
P(tdrightcolspan2)= "<td align=\"right\" colspan=\"2\">";
P(centercolspan2)= "align=\"center\" colspan=\"2\">";
P(centercolspan3)= "align=\"center\" colspan=\"3\">";
P(colspan4)="colspan=\"4\">";
P(tdwidthbgcolorFFFF00)="<td width=\"40\" bgcolor=#FFFF00 ";

P(aligncenteron)="align=\"center\">ON";
P(aligncenteroff)="align=\"center\">OFF";
P(hrefon)="<a href=\"on?";
P(hrefoff)="<a href=\"off?";
P(hrefonr)="<a href=\"onrh?";
P(hrefoffr)="<a href=\"offrh?";
P(ona_f)="\">ON</a>";
P(offa_f)="\">OFF</a>";
P(celsius)=" ºC";          
P(hr)="<hr>";  
P(dospuntos)=":";
P(resetcontl)="<a href=\"sy.htm?act=rl";     
P(resetcontp)="<a href=\"sy.htm?act=rp";     
P(a_f)="</a>";          
P(blancos4)="    ";
P(punto)=".";
//P(coma)=",";
P(puntoycoma)=";";
P(porciento)=" %";
P(letraa)="a";
P(letrad)="d";
P(letrae)="e";
P(letraf)="f";
P(letraI)="I";
P(letrak)="k";
P(letran)="n";
P(letraN)="N";
P(letraP)="P";
P(letrar)="r";
P(letras)="s";
P(letrav)="v";
P(letrasred)="rdi";
P(letrasrsd)="rds";
P(ed)="ed";
P(sd)="sd";
P(rs)="rs";
P(ra)="ra";
P(rk)="rk";
P(rr)="rr";
P(N2puntos)="N: "; 
P(C2puntos)="C: "; 
P(paren_i)="(";
P(paren_f)=")";
P(paren_f1b)=") ";
P(paren_f2b)=")  ";
P(corchete_i)=" [";
P(corchete_f)="]";
P(parenguion)=")-";
P(mayorparen)=">(";
P(optionvalue0)="<OPTION value=\"0\"";  
P(optionvalue1)="<OPTION value=\"1\"";  
P(optionvalue2)="<OPTION value=\"2\"";  
P(optionvalue4)="<OPTION value=\"2\"";  
P(optionvalue5)="<OPTION value=\"2\"";  
P(optionvalue255)="<OPTION value=\"255\"";
P(barraa)="a";
P(barradi)="di";
P(barrads)="ds";
P(barrared)="red";
P(barrars)="rs";
P(barrarsd)="rsd";
P(barrara)="ra";
P(barrarr)="rr";
P(barrado)="do";
P(barras)="s";
P(llave_i)="{";
P(llave_f)="}";
P(comablanco)=", ";
P(tdalignright)="<td align=\"right\">";            
P(tralignright)="<tr align=\"right\">";            
P(guion)="-";            
P(tdcolspan2)="<td colspan=\"2\">";
P(tdcolspan3)="<td colspan=\"3\">";
P(tdcolspan4)="<td colspan=\"4\">";        
P(tdcolspan5)="<td colspan=\"5\">";        
P(tdcolspan7)="<td colspan=\"7\">";
P(inputcheckbox)="<input type=\"checkbox\" name=\""; 
P(dema)="DEMA";
P(chacon)="CHACON D-IO";
P(conuco)="CONUCO";
//P(kaku)="KAKU";
//P(blokker)="BLOKKER";
//P(elro)="ELRO";
P(n) = "n";
P(barra) = "/";
P(barraatras)="\"";
//P(cero) ="0";
//P(uno) = "1";
//P(dos) = "2";
//P(tres) = "3";
P(cuatro) = "4";
P(cinco) = "5";
P(seis) = "6";
P(bytes) = "bytes";            
P(Bytes) = "Bytes";            
P(elconuco) = "El Conuco Labs";
P(dyndnsNOIP)="https://dynupdate.no-ip.com";
P(noip)="NO-IP";
P(dyndns)="DynDNS";
P(blancoC)=" C";
P(blancoIP)=" IP:";
P(menor)="<";
P(mayor)=">";
P(tRTC)="RTC ";
P(mdospuntosp)=" M:P";
P(mailHELO)="EHLO 1";
P(mailAUTH)="AUTH LOGIN";
P(mailFrom)="MAIL From: ";
P(mailRCPT)="RCPT To: ";
P(mailDATA)="DATA";
P(mailFROM)="From: Conuco Controller <";
P(mailTO)="To: ";
P(mailSubject)="Subject: ";
P(mailQUIT)="QUIT";
P(mailQuitrin)="QUIT\ri\n";  
P(mailelconuco)="Conuco Controller.";    
P(ttestEEPROM)="EEPROM";
P(escribirceros)="Writing/Reading zero";
P(escribir015)="Writing/Reading values 0-15";
P(href_i)="<a href=";
P(href_cierre)=">";
P(href_f)="</a>";
P(chdir01)="<a href=\"cd?";
P(chdir02)="\">";
P(chdir03)="</a>";
P(showfile01)="<a href=\"sf?";
P(showfile02)="\">";
P(showfile03)="</a>";
P(deletefile01)="<a href=\"df?";
P(deletefile02)="\">";
P(deletefile03)="</a>";
P(ready)="*****  Ready  *****";
P(avisoINI)="=== INI ===";
P(avisoFIN)="=== FIN ===";
P(dht11)="DHT11";
P(dht21)="DHT21";
P(dht22)="DHT22";
P(error)="ERROR";
P(blancoERROR)=" ERROR";
P(blancoOK)=" OK";
P(Panel)="Panel";
P(act)="act";
P(rsteex)="rsteex";
P(shee)="shee";
P(backup)="backup";
P(restore)="restore";
P(deletefile)="deletefile";
P(ssehtml)="sse.htm";
P(sfhtml)="sf.htm";
P(tjson)="json";
P(puntoflecha)=".  -->";
P(mux)="MUX";
P(dirip)="IP";
P(ssid)="SSID";
P(APssid)="SSID";
P(raizdirip)="192.168.4.nnn";
P(prefixIP)="192.168.4.";
P(di)="di";
P(ds)="ds";
P(a)="a";
P(s)="s";
P(namelogEve)="eventos.log";
P(raizgwipdefault)="192.168.4.";
P(cmdgetmainr)="GET /mainr";
P(cmdgetyourid)="GET /yourid";
P(cmdgetjson)="GET /json";
P(cmdgetjsonr)="GET /jsonr";
P(cmdmyid)="INF myid=";
P(cmdsetgwip)="SET /gwip";
P(cmdonr)="SET /on";
P(cmdoffr)="SET /off";
P(cmdsetidset)="SET /idset=";
P(timeout)="Timeout";
P(srnohtm) = "srno.htm";
P(openserverok)="Open Server OK";
P(openservererror)="Open Server ERROR";
P(srnhtm)="srn.htm";
P(sprghtm)="sprg.htm";
P(slkhtm)="slk.htm";
P(seschtm)="sesc.htm";
P(svhtm)="sv.htm";
P(sclhtm)="scl.htm";
P(snehtm)="sne.htm";
P(sdhtm)="sd.htm";
P(sthtm)="st.htm";
P(sahtm)="sa.htm";
P(sedhtm)="sed.htm";
P(ssdhtm)="ssd.htm";
P(sshtm)="ss.htm";
P(srlhtm)="srl.htm";
P(ssehtm)="sse.htm";
P(sfhtm)="sf.htm";
P(srhtm)="sr.htm";
P(srthtm)="srt.htm";
P(srahtm)="sra.htm";
P(srihtm)="sri.htm";
P(srohtm)="sro.htm";
P(srrhtm)="srr.htm";
P(rsthtm)="rst.htm";
P(slghtm)="slg.htm";
P(sflhtm)="sfl.htm";
P(savhtm)="sav.htm";
P(Comando)="Comando";
P(GetConfig)="GET Config";
P(SetConfig)="SET Config";
P(RESET)="Reset";
P(GetCIPMode)="Get CIP mode";
P(GetCIPStatus)="Get CIP Status";
P(CipStart)="CIP Start";
P(GetCWMode)="Get CW mode";
P(GetCWLif)="Get CWLIF";
P(SetCWMode)="Set CW mode";
P(GetCWSAP)="Get CWSAP (AP parm.)";
P(SetCWSAP)="Set CWSAP (AP parm.)";
P(GetAP)="Get AP List";
P(GetIP)="Get IP";
P(GetMUX)="Get MUX";
P(JoinAP)="Join AP";
P(QuitAP)="Quit AP";
P(GetVer)="Get Version";
P(cwmode)="CW MODE";  
P(cwmode1)="STA";  
P(cwmode2)="AP";  
P(cwmode3)="STA+AP";  
P(wifiSSid)="Connect to: SSID";
P(wifiPass)="Connect to: PASS";
P(mySSid)="AP SSID";
P(myPass)="AP PASS";
P(CipClose)="CIP Close";
P(GetCIOBaud)="Get CIO Baud";
P(tcpserver)="TCP server";
P(tcpport)="TCP port";
P(GetHTTP)="Get HTTP";
P(OpenServer)="Open Server";
P(CloseServer)="Close Server";
P(sendgjson)="Send Get /json";
P(send1a10)="Send 1234567890";
P(noencriptyon)="No encryption";   
P(wep)="WEP";   
P(wpapsk)="WPA_PSK";   
P(wpa2psk)="WPA2_PSK";   
P(wpawpa2psk)="WPA_WPA2_PSK";   
P(lineadoble)="============";
P(ENTANA)="ENT ANA ";
P(muxmode)=" MUX mode";
P(cipstatus)=" CIPStatus";
P(treset)="  Reset ";

