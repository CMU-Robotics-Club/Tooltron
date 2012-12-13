<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.2">
<drawing>
<settings>
<setting alwaysvectorfont="yes"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="1" fill="3" visible="no" active="no"/>
<layer number="3" name="Route3" color="4" fill="3" visible="no" active="no"/>
<layer number="14" name="Route14" color="1" fill="6" visible="no" active="no"/>
<layer number="15" name="Route15" color="4" fill="6" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="npol_custom">
<packages>
<package name="1X4_HEADER">
<pad name="2" x="-1.27" y="0" drill="1.1" diameter="1.9304"/>
<pad name="3" x="1.27" y="0" drill="1.1" diameter="1.9304"/>
<pad name="4" x="3.81" y="0" drill="1.1" diameter="1.9304"/>
<pad name="1" x="-3.81" y="0" drill="1.1" diameter="1.9304"/>
<wire x1="-5.08" y1="1.27" x2="-5.08" y2="-1.27" width="0.127" layer="21"/>
</package>
<package name="1-406541-1">
<pad name="G2" x="-6.35" y="0" drill="3.2512"/>
<pad name="G1" x="6.35" y="0" drill="3.2512"/>
<pad name="1" x="3.556" y="2.54" drill="0.9"/>
<pad name="3" x="1.524" y="2.54" drill="0.9"/>
<pad name="5" x="-0.508" y="2.54" drill="0.9"/>
<pad name="7" x="-2.54" y="2.54" drill="0.9"/>
<pad name="8" x="-3.556" y="4.318" drill="0.9"/>
<pad name="6" x="-1.524" y="4.318" drill="0.9"/>
<pad name="4" x="0.508" y="4.318" drill="0.9"/>
<pad name="2" x="2.54" y="4.318" drill="0.9"/>
<pad name="G3" x="8.128" y="3.429" drill="1.6"/>
<pad name="G4" x="-8.128" y="3.429" drill="1.6"/>
<wire x1="-8.128" y1="-2.54" x2="-8.128" y2="-5.2832" width="0.254" layer="21"/>
<wire x1="-8.128" y1="-5.2832" x2="8.128" y2="-5.2832" width="0.254" layer="21"/>
<wire x1="8.128" y1="-5.2832" x2="8.128" y2="-2.54" width="0.254" layer="21"/>
<wire x1="-8.128" y1="5.08" x2="-8.128" y2="10.4648" width="0.254" layer="21"/>
<wire x1="-8.128" y1="10.4648" x2="8.128" y2="10.4648" width="0.254" layer="21"/>
<wire x1="8.128" y1="10.4648" x2="8.128" y2="5.08" width="0.254" layer="21"/>
<text x="-8.89" y="-5.08" size="1.27" layer="21" rot="R90">&gt;NAME</text>
</package>
</packages>
<symbols>
<symbol name="RFID_SERIAL_R/W">
<pin name="VCC" x="-10.16" y="2.54" length="middle"/>
<pin name="SIN" x="-10.16" y="0" length="middle"/>
<pin name="SOUT" x="-10.16" y="-2.54" length="middle"/>
<pin name="GND" x="-10.16" y="-5.08" length="middle"/>
<wire x1="-5.08" y1="5.08" x2="-5.08" y2="-7.62" width="0.254" layer="94"/>
<wire x1="-5.08" y1="-7.62" x2="15.24" y2="-7.62" width="0.254" layer="94"/>
<wire x1="15.24" y1="-7.62" x2="15.24" y2="5.08" width="0.254" layer="94"/>
<wire x1="15.24" y1="5.08" x2="-5.08" y2="5.08" width="0.254" layer="94"/>
<wire x1="10.16" y1="2.54" x2="12.7" y2="0" width="0.254" layer="94"/>
<wire x1="12.7" y1="0" x2="12.7" y2="-2.54" width="0.254" layer="94"/>
<wire x1="12.7" y1="-2.54" x2="10.16" y2="-5.08" width="0.254" layer="94"/>
<wire x1="10.16" y1="-5.08" x2="7.62" y2="-5.08" width="0.254" layer="94"/>
<wire x1="7.62" y1="-5.08" x2="5.08" y2="-2.54" width="0.254" layer="94"/>
<wire x1="5.08" y1="-2.54" x2="5.08" y2="0" width="0.254" layer="94"/>
<wire x1="5.08" y1="0" x2="7.62" y2="2.54" width="0.254" layer="94"/>
<wire x1="7.62" y1="2.54" x2="10.16" y2="2.54" width="0.254" layer="94"/>
<text x="-5.08" y="5.08" size="1.778" layer="94">&gt;NAME</text>
</symbol>
<symbol name="RJ_45">
<pin name="4" x="-2.54" y="-5.08" length="middle" rot="R90"/>
<pin name="3" x="-5.08" y="-5.08" length="middle" rot="R90"/>
<pin name="2" x="-7.62" y="-5.08" length="middle" rot="R90"/>
<pin name="1" x="-10.16" y="-5.08" length="middle" rot="R90"/>
<pin name="5" x="0" y="-5.08" length="middle" rot="R90"/>
<pin name="6" x="2.54" y="-5.08" length="middle" rot="R90"/>
<pin name="7" x="5.08" y="-5.08" length="middle" rot="R90"/>
<pin name="8" x="7.62" y="-5.08" length="middle" rot="R90"/>
<pin name="G1" x="-17.78" y="5.08" length="middle"/>
<pin name="G2" x="-17.78" y="7.62" length="middle"/>
<pin name="G3" x="15.24" y="5.08" length="middle" rot="R180"/>
<pin name="G4" x="15.24" y="7.62" length="middle" rot="R180"/>
<wire x1="-12.7" y1="12.7" x2="-12.7" y2="0" width="0.254" layer="94"/>
<wire x1="-12.7" y1="0" x2="10.16" y2="0" width="0.254" layer="94"/>
<wire x1="10.16" y1="0" x2="10.16" y2="12.7" width="0.254" layer="94"/>
<wire x1="10.16" y1="12.7" x2="-12.7" y2="12.7" width="0.254" layer="94"/>
<wire x1="-5.08" y1="10.16" x2="-5.08" y2="7.62" width="0.254" layer="94"/>
<wire x1="-5.08" y1="7.62" x2="-7.62" y2="7.62" width="0.254" layer="94"/>
<wire x1="-7.62" y1="7.62" x2="-7.62" y2="5.08" width="0.254" layer="94"/>
<wire x1="-7.62" y1="5.08" x2="5.08" y2="5.08" width="0.254" layer="94"/>
<wire x1="5.08" y1="5.08" x2="5.08" y2="7.62" width="0.254" layer="94"/>
<wire x1="5.08" y1="7.62" x2="2.54" y2="7.62" width="0.254" layer="94"/>
<wire x1="2.54" y1="7.62" x2="2.54" y2="10.16" width="0.254" layer="94"/>
<wire x1="2.54" y1="10.16" x2="-5.08" y2="10.16" width="0.254" layer="94"/>
<text x="-12.7" y="12.7" size="1.27" layer="94">&gt;NAME</text>
<text x="2.54" y="12.7" size="1.27" layer="94">&gt;VALUE</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="RFID_SERIAL_R/W">
<gates>
<gate name="G$1" symbol="RFID_SERIAL_R/W" x="2.54" y="0"/>
</gates>
<devices>
<device name="" package="1X4_HEADER">
<connects>
<connect gate="G$1" pin="GND" pad="4"/>
<connect gate="G$1" pin="SIN" pad="2"/>
<connect gate="G$1" pin="SOUT" pad="3"/>
<connect gate="G$1" pin="VCC" pad="1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="RJ45_406541">
<description>RJ45 Jack
TE Connectivity 1-406541-1
Digikey A97716-ND</description>
<gates>
<gate name="G$1" symbol="RJ_45" x="2.54" y="0"/>
</gates>
<devices>
<device name="" package="1-406541-1">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="G1" pad="G1"/>
<connect gate="G$1" pin="G2" pad="G2"/>
<connect gate="G$1" pin="G3" pad="G3"/>
<connect gate="G$1" pin="G4" pad="G4"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply1">
<description>&lt;b&gt;Supply Symbols&lt;/b&gt;&lt;p&gt;
 GND, VCC, 0V, +5V, -5V, etc.&lt;p&gt;
 Please keep in mind, that these devices are necessary for the
 automatic wiring of the supply signals.&lt;p&gt;
 The pin name defined in the symbol is identical to the net which is to be wired automatically.&lt;p&gt;
 In this library the device names are the same as the pin names of the symbols, therefore the correct signal names appear next to the supply symbols in the schematic.&lt;p&gt;
 &lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="GND">
<wire x1="-1.905" y1="0" x2="1.905" y2="0" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="GND" prefix="GND">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="GND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="led">
<description>&lt;b&gt;LEDs&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;&lt;br&gt;
Extended by Federico Battaglin &lt;author&gt;&amp;lt;federico.rd@fdpinternational.com&amp;gt;&lt;/author&gt; with DUOLED</description>
<packages>
<package name="DUOLED-C-5MM">
<description>&lt;B&gt;DUO LED&lt;/B&gt;&lt;P&gt;
2 colors (red/green, red/yellow), 5 mm, round, common Cathode&lt;BR&gt;
&lt;I&gt;www.ligitek.com&lt;/I&gt; or other producers.</description>
<wire x1="-2.794" y1="0.8636" x2="-2.794" y2="-0.8636" width="0.254" layer="51" curve="34.351807"/>
<wire x1="2.3876" y1="-0.8636" x2="2.3876" y2="0.8636" width="0.1524" layer="51" curve="39.77033"/>
<wire x1="2.54" y1="1.4478" x2="2.54" y2="0.889" width="0.254" layer="21"/>
<wire x1="-2.3876" y1="-0.8636" x2="2.3876" y2="-0.8636" width="0.1524" layer="21" curve="140.22967"/>
<wire x1="-2.7933" y1="0.8634" x2="2.54" y2="1.4478" width="0.254" layer="21" curve="-133.151599"/>
<wire x1="-2.3876" y1="0.8636" x2="-2.3876" y2="-0.8636" width="0.1524" layer="51" curve="39.77033"/>
<wire x1="-2.794" y1="-0.8636" x2="2.5407" y2="-1.4482" width="0.254" layer="21" curve="133.133633"/>
<wire x1="-2.3876" y1="0.8636" x2="2.3876" y2="0.8636" width="0.1524" layer="21" curve="-140.22967"/>
<wire x1="2.54" y1="0.9398" x2="2.54" y2="-0.889" width="0.254" layer="51"/>
<wire x1="2.54" y1="-0.889" x2="2.54" y2="-1.4478" width="0.254" layer="21"/>
<wire x1="-1.143" y1="0" x2="0" y2="1.143" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-1.143" x2="1.143" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="-1.651" y1="0" x2="0" y2="1.651" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-1.651" x2="1.651" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="-2.159" y1="0" x2="0" y2="2.159" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-2.159" x2="2.159" y2="0" width="0.1524" layer="51" curve="90"/>
<pad name="AX" x="-2.54" y="0" drill="0.8128" shape="long" rot="R90"/>
<pad name="C" x="0" y="0" drill="0.8128" shape="long" rot="R90"/>
<pad name="AR" x="2.54" y="0" drill="0.8128" shape="long" rot="R90"/>
<text x="3.81" y="2.54" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="3.81" y="-2.54" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="DUOLED-RG-C">
<wire x1="1.27" y1="5.08" x2="0" y2="2.54" width="0.254" layer="94"/>
<wire x1="0" y1="2.54" x2="-1.27" y2="5.08" width="0.254" layer="94"/>
<wire x1="1.27" y1="2.54" x2="0" y2="2.54" width="0.254" layer="94"/>
<wire x1="0" y1="2.54" x2="-1.27" y2="2.54" width="0.254" layer="94"/>
<wire x1="1.27" y1="5.08" x2="0" y2="5.08" width="0.254" layer="94"/>
<wire x1="0" y1="5.08" x2="-1.27" y2="5.08" width="0.254" layer="94"/>
<wire x1="0" y1="5.08" x2="0" y2="2.54" width="0.1524" layer="94"/>
<wire x1="-2.032" y1="4.318" x2="-3.429" y2="2.921" width="0.1524" layer="94"/>
<wire x1="-1.905" y1="3.175" x2="-3.302" y2="1.778" width="0.1524" layer="94"/>
<wire x1="-1.27" y1="-5.08" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="1.27" y2="-5.08" width="0.254" layer="94"/>
<wire x1="-1.27" y1="-2.54" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="1.27" y2="-2.54" width="0.254" layer="94"/>
<wire x1="-1.27" y1="-5.08" x2="0" y2="-5.08" width="0.254" layer="94"/>
<wire x1="0" y1="-5.08" x2="1.27" y2="-5.08" width="0.254" layer="94"/>
<wire x1="0" y1="-5.08" x2="0" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="2.032" y1="-4.318" x2="3.429" y2="-2.921" width="0.1524" layer="94"/>
<wire x1="1.905" y1="-3.175" x2="3.302" y2="-1.778" width="0.1524" layer="94"/>
<wire x1="0" y1="2.54" x2="0" y2="-2.54" width="0.1524" layer="94"/>
<circle x="0" y="0" radius="0.254" width="0.4064" layer="94"/>
<text x="6.35" y="-5.08" size="1.778" layer="95" rot="R90">&gt;NAME</text>
<text x="8.89" y="-5.08" size="1.778" layer="96" rot="R90">&gt;VALUE</text>
<text x="2.54" y="3.81" size="1.016" layer="94" ratio="10" rot="R90">red</text>
<text x="-2.54" y="-5.08" size="1.016" layer="94" ratio="10" rot="R90">green</text>
<pin name="C" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
<pin name="AR" x="0" y="7.62" visible="off" length="short" direction="pas" rot="R270"/>
<pin name="AG" x="0" y="-7.62" visible="off" length="short" direction="pas" rot="R90"/>
<polygon width="0.1524" layer="94">
<vertex x="-3.429" y="2.921"/>
<vertex x="-3.048" y="3.81"/>
<vertex x="-2.54" y="3.302"/>
</polygon>
<polygon width="0.1524" layer="94">
<vertex x="-3.302" y="1.778"/>
<vertex x="-2.921" y="2.667"/>
<vertex x="-2.413" y="2.159"/>
</polygon>
<polygon width="0.1524" layer="94">
<vertex x="2.413" y="-2.159"/>
<vertex x="3.302" y="-1.778"/>
<vertex x="2.921" y="-2.667"/>
</polygon>
<polygon width="0.1524" layer="94">
<vertex x="2.54" y="-3.302"/>
<vertex x="3.429" y="-2.921"/>
<vertex x="3.048" y="-3.81"/>
</polygon>
</symbol>
</symbols>
<devicesets>
<deviceset name="DUOLED-RG-C" prefix="LD" uservalue="yes">
<description>&lt;B&gt;DUO LED&lt;/B&gt;&lt;P&gt;
2 colors (red/green), 5 mm, round, common Cathode&lt;BR&gt;
&lt;I&gt;www.ligitek.com&lt;/I&gt; or other producers.</description>
<gates>
<gate name="G$1" symbol="DUOLED-RG-C" x="0" y="0"/>
</gates>
<devices>
<device name="" package="DUOLED-C-5MM">
<connects>
<connect gate="G$1" pin="AG" pad="AX"/>
<connect gate="G$1" pin="AR" pad="AR"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="J1" library="npol_custom" deviceset="RFID_SERIAL_R/W" device=""/>
<part name="J2" library="npol_custom" deviceset="RJ45_406541" device=""/>
<part name="GND3" library="supply1" deviceset="GND" device=""/>
<part name="GND4" library="supply1" deviceset="GND" device=""/>
<part name="GND1" library="supply1" deviceset="GND" device=""/>
<part name="LD1" library="led" deviceset="DUOLED-RG-C" device=""/>
<part name="GND2" library="supply1" deviceset="GND" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<wire x1="0" y1="0" x2="48.26" y2="0" width="0.1524" layer="97"/>
<wire x1="48.26" y1="0" x2="48.26" y2="-30.48" width="0.1524" layer="97"/>
<wire x1="48.26" y1="-30.48" x2="48.26" y2="-53.34" width="0.1524" layer="97"/>
<wire x1="48.26" y1="-53.34" x2="48.26" y2="-76.2" width="0.1524" layer="97"/>
<wire x1="48.26" y1="-76.2" x2="0" y2="-76.2" width="0.1524" layer="97"/>
<wire x1="0" y1="-76.2" x2="0" y2="0" width="0.1524" layer="97"/>
<wire x1="48.26" y1="-30.48" x2="99.06" y2="-30.48" width="0.1524" layer="97"/>
<wire x1="99.06" y1="-30.48" x2="99.06" y2="-53.34" width="0.1524" layer="97"/>
<wire x1="99.06" y1="-53.34" x2="81.28" y2="-53.34" width="0.1524" layer="97"/>
<wire x1="48.26" y1="-53.34" x2="81.28" y2="-53.34" width="0.1524" layer="97"/>
<wire x1="81.28" y1="-53.34" x2="81.28" y2="-76.2" width="0.1524" layer="97"/>
<wire x1="81.28" y1="-76.2" x2="48.26" y2="-76.2" width="0.1524" layer="97"/>
<text x="15.24" y="-2.54" size="1.778" layer="97">RJ-45 Input</text>
<text x="68.58" y="-33.02" size="1.778" layer="97">RFID R/W</text>
<text x="55.88" y="-55.88" size="1.778" layer="97">Status LED</text>
</plain>
<instances>
<instance part="J1" gate="G$1" x="78.74" y="-43.18"/>
<instance part="J2" gate="G$1" x="25.4" y="-17.78"/>
<instance part="GND3" gate="1" x="68.58" y="-50.8"/>
<instance part="GND4" gate="1" x="5.08" y="-15.24"/>
<instance part="GND1" gate="1" x="43.18" y="-15.24"/>
<instance part="LD1" gate="G$1" x="68.58" y="-66.04" rot="R180"/>
<instance part="GND2" gate="1" x="76.2" y="-68.58"/>
</instances>
<busses>
</busses>
<nets>
<net name="GND" class="0">
<segment>
<pinref part="J1" gate="G$1" pin="GND"/>
<pinref part="GND3" gate="1" pin="GND"/>
</segment>
<segment>
<pinref part="J2" gate="G$1" pin="G2"/>
<pinref part="J2" gate="G$1" pin="G1"/>
<wire x1="7.62" y1="-10.16" x2="7.62" y2="-12.7" width="0.1524" layer="91"/>
<junction x="7.62" y="-12.7"/>
<pinref part="J2" gate="G$1" pin="3"/>
<wire x1="20.32" y1="-22.86" x2="20.32" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-35.56" x2="7.62" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="7.62" y1="-35.56" x2="7.62" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-35.56" x2="30.48" y2="-35.56" width="0.1524" layer="91"/>
<junction x="20.32" y="-35.56"/>
<pinref part="J2" gate="G$1" pin="7"/>
<wire x1="30.48" y1="-35.56" x2="30.48" y2="-22.86" width="0.1524" layer="91"/>
<pinref part="J2" gate="G$1" pin="G4"/>
<pinref part="J2" gate="G$1" pin="G3"/>
<wire x1="40.64" y1="-10.16" x2="40.64" y2="-12.7" width="0.1524" layer="91"/>
<junction x="40.64" y="-12.7"/>
<wire x1="30.48" y1="-35.56" x2="40.64" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="40.64" y1="-35.56" x2="40.64" y2="-12.7" width="0.1524" layer="91"/>
<junction x="30.48" y="-35.56"/>
<pinref part="GND4" gate="1" pin="GND"/>
<wire x1="5.08" y1="-12.7" x2="7.62" y2="-12.7" width="0.1524" layer="91"/>
<pinref part="GND1" gate="1" pin="GND"/>
<wire x1="40.64" y1="-12.7" x2="43.18" y2="-12.7" width="0.1524" layer="91"/>
<label x="20.32" y="-33.02" size="1.778" layer="95" rot="R90"/>
<label x="30.48" y="-33.02" size="1.778" layer="95" rot="R90"/>
</segment>
<segment>
<pinref part="LD1" gate="G$1" pin="C"/>
<pinref part="GND2" gate="1" pin="GND"/>
<wire x1="71.12" y1="-66.04" x2="76.2" y2="-66.04" width="0.1524" layer="91"/>
</segment>
</net>
<net name="5VDC" class="0">
<segment>
<pinref part="J2" gate="G$1" pin="1"/>
<pinref part="J2" gate="G$1" pin="2"/>
<wire x1="15.24" y1="-22.86" x2="17.78" y2="-22.86" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="VCC"/>
<wire x1="68.58" y1="-40.64" x2="17.78" y2="-40.64" width="0.1524" layer="91"/>
<wire x1="17.78" y1="-40.64" x2="17.78" y2="-22.86" width="0.1524" layer="91"/>
<junction x="17.78" y="-22.86"/>
<label x="17.78" y="-33.02" size="1.778" layer="95" rot="R90"/>
</segment>
</net>
<net name="RFTX" class="0">
<segment>
<pinref part="J2" gate="G$1" pin="6"/>
<wire x1="27.94" y1="-22.86" x2="27.94" y2="-45.72" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="SOUT"/>
<wire x1="27.94" y1="-45.72" x2="68.58" y2="-45.72" width="0.1524" layer="91"/>
<label x="27.94" y="-33.02" size="1.778" layer="95" rot="R90"/>
</segment>
</net>
<net name="RFRX" class="0">
<segment>
<pinref part="J1" gate="G$1" pin="SIN"/>
<pinref part="J2" gate="G$1" pin="8"/>
<wire x1="68.58" y1="-43.18" x2="33.02" y2="-43.18" width="0.1524" layer="91"/>
<wire x1="33.02" y1="-43.18" x2="33.02" y2="-22.86" width="0.1524" layer="91"/>
<label x="33.02" y="-33.02" size="1.778" layer="95" rot="R90"/>
</segment>
</net>
<net name="RED" class="0">
<segment>
<pinref part="J2" gate="G$1" pin="4"/>
<wire x1="22.86" y1="-22.86" x2="22.86" y2="-73.66" width="0.1524" layer="91"/>
<pinref part="LD1" gate="G$1" pin="AR"/>
<wire x1="22.86" y1="-73.66" x2="68.58" y2="-73.66" width="0.1524" layer="91"/>
<label x="22.86" y="-33.02" size="1.778" layer="95" rot="R90"/>
</segment>
</net>
<net name="GREEN" class="0">
<segment>
<pinref part="LD1" gate="G$1" pin="AG"/>
<pinref part="J2" gate="G$1" pin="5"/>
<wire x1="68.58" y1="-58.42" x2="25.4" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-58.42" x2="25.4" y2="-22.86" width="0.1524" layer="91"/>
<label x="25.4" y="-33.02" size="1.778" layer="95" rot="R90"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
