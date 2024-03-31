<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.7.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="16" fill="1" visible="no" active="no"/>
<layer number="3" name="Route3" color="17" fill="1" visible="no" active="no"/>
<layer number="4" name="Route4" color="18" fill="1" visible="no" active="no"/>
<layer number="5" name="Route5" color="19" fill="1" visible="no" active="no"/>
<layer number="6" name="Route6" color="25" fill="1" visible="no" active="no"/>
<layer number="7" name="Route7" color="26" fill="1" visible="no" active="no"/>
<layer number="8" name="Route8" color="27" fill="1" visible="no" active="no"/>
<layer number="9" name="Route9" color="28" fill="1" visible="no" active="no"/>
<layer number="10" name="Route10" color="29" fill="1" visible="no" active="no"/>
<layer number="11" name="Route11" color="30" fill="1" visible="no" active="no"/>
<layer number="12" name="Route12" color="20" fill="1" visible="no" active="no"/>
<layer number="13" name="Route13" color="21" fill="1" visible="no" active="no"/>
<layer number="14" name="Route14" color="22" fill="1" visible="no" active="no"/>
<layer number="15" name="Route15" color="23" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="24" fill="1" visible="no" active="no"/>
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
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="99" name="SpiceOrder" color="7" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="Connector">
<description>&lt;b&gt;Pin Headers,Terminal blocks, D-Sub, Backplane, FFC/FPC, Socket</description>
<packages>
<package name="B4B-XH-A" urn="urn:adsk.eagle:footprint:24957617/2">
<pad name="1" x="0" y="0" drill="0.84"/>
<pad name="2" x="2.5" y="0" drill="0.84"/>
<pad name="3" x="5" y="0" drill="0.84"/>
<pad name="4" x="7.5" y="0" drill="0.84"/>
<wire x1="-2.45" y1="3.45" x2="9.95" y2="3.45" width="0.1524" layer="21"/>
<wire x1="-2.45" y1="-2.3" x2="9.95" y2="-2.3" width="0.1524" layer="21"/>
<wire x1="-2.45" y1="3.45" x2="-2.45" y2="-2.3" width="0.1524" layer="21"/>
<wire x1="9.95" y1="3.45" x2="9.95" y2="-2.3" width="0.1524" layer="21"/>
<text x="2.54" y="5.08" size="1" layer="25" align="center">&gt;NAME</text>
<text x="2.54" y="-3.81" size="1" layer="27" align="center">&gt;VALUE</text>
</package>
<package name="B2B-XH-A" urn="urn:adsk.eagle:footprint:24957611/2">
<pad name="1" x="0" y="0" drill="0.84"/>
<pad name="2" x="2.5" y="0" drill="0.84"/>
<wire x1="-2.45" y1="2.35" x2="4.95" y2="2.35" width="0.1524" layer="21"/>
<wire x1="-2.45" y1="2.35" x2="-2.45" y2="-3.4" width="0.1524" layer="21"/>
<wire x1="4.95" y1="2.35" x2="4.95" y2="-3.4" width="0.1524" layer="21"/>
<wire x1="-2.45" y1="-3.4" x2="4.95" y2="-3.4" width="0.1524" layer="21"/>
<text x="1.27" y="3.81" size="1" layer="25" align="center">&gt;NAME</text>
<text x="1.27" y="-5.08" size="1" layer="27" align="center">&gt;VALUE</text>
</package>
</packages>
<packages3d>
<package3d name="B4B-XH-A" urn="urn:adsk.eagle:package:24957647/3" type="model">
<packageinstances>
<packageinstance name="B4B-XH-A"/>
</packageinstances>
</package3d>
<package3d name="B2B-XH-A" urn="urn:adsk.eagle:package:24957641/6" type="model">
<packageinstances>
<packageinstance name="B2B-XH-A"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="TERMBLK_4-1" urn="urn:adsk.eagle:symbol:24957596/4">
<wire x1="-2.54" y1="3.81" x2="2.54" y2="3.81" width="0.1524" layer="94"/>
<wire x1="2.54" y1="3.81" x2="2.54" y2="-6.35" width="0.1524" layer="94"/>
<wire x1="2.54" y1="-6.35" x2="-2.54" y2="-6.35" width="0.1524" layer="94"/>
<wire x1="-2.54" y1="-6.35" x2="-2.54" y2="3.81" width="0.1524" layer="94"/>
<pin name="1" x="-5.08" y="2.54" length="short"/>
<pin name="2" x="-5.08" y="0" length="short"/>
<pin name="3" x="-5.08" y="-2.54" length="short"/>
<pin name="4" x="-5.08" y="-5.08" length="short"/>
<text x="0" y="-6.604" size="1.778" layer="96" align="top-center">&gt;VALUE</text>
<text x="0" y="4.064" size="1.778" layer="95" align="bottom-center">&gt;NAME</text>
</symbol>
<symbol name="TERMBLK_2" urn="urn:adsk.eagle:symbol:24957587/3">
<pin name="1" x="-5.08" y="2.54" length="short"/>
<pin name="2" x="-5.08" y="0" length="short"/>
<text x="0" y="-2.794" size="1.778" layer="96" align="top-center">&gt;VALUE</text>
<text x="0" y="5.334" size="1.778" layer="95" align="bottom-center">&gt;NAME</text>
<wire x1="-2.54" y1="-2.54" x2="-2.54" y2="5.08" width="0.1524" layer="94"/>
<wire x1="-2.54" y1="5.08" x2="2.54" y2="5.08" width="0.1524" layer="94"/>
<wire x1="2.54" y1="5.08" x2="2.54" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="2.54" y1="-2.54" x2="-2.54" y2="-2.54" width="0.1524" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="B4B-XH-A" urn="urn:adsk.eagle:component:24957705/8" prefix="J">
<description>XH Connector Top Entry - 4POS
&lt;br&gt;Details see: &lt;a href="http://www.jst-mfg.com/product/pdf/eng/eXH.pdf"&gt;Datasheet&lt;/a&gt;&lt;p&gt;</description>
<gates>
<gate name="G$1" symbol="TERMBLK_4-1" x="0" y="0"/>
</gates>
<devices>
<device name="" package="B4B-XH-A">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:24957647/3"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="CATEGORY" value="Header" constant="no"/>
<attribute name="DSECRIPTION" value="Connector Header Through Hole 4 position 0.098&quot; (2.50mm) " constant="no"/>
<attribute name="MANUFACTURER" value="JST Sales America Inc." constant="no"/>
<attribute name="MPN" value="B4B-XH-A(LF)(SN)" constant="no"/>
<attribute name="OPERATING_TEMPERATURE" value="-25 C to 85 C" constant="no"/>
<attribute name="PART_STATUS" value="ACTIVE" constant="no"/>
<attribute name="PITCH" value="0.098&quot; (2.50mm) " constant="no"/>
<attribute name="ROHS_COMPLIANCE" value="ROHS3 Compliant " constant="no"/>
<attribute name="SERIES" value="XH" constant="no"/>
<attribute name="SUB_CATEGORY" value="Rectangular Connectors " constant="no"/>
<attribute name="TYPE" value="Header, Male Pin, Board to Cable/Wire, Through Hole" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="B2B-XH-A" urn="urn:adsk.eagle:component:24957700/8" prefix="J">
<description>XH Connector Top Entry - 2POS
&lt;br&gt;Details see: &lt;a href="http://www.jst-mfg.com/product/pdf/eng/eXH.pdf"&gt;Datasheet&lt;/a&gt;&lt;p&gt;</description>
<gates>
<gate name="G$1" symbol="TERMBLK_2" x="0" y="0"/>
</gates>
<devices>
<device name="" package="B2B-XH-A">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:24957641/6"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="CATEGORY" value="Header " constant="no"/>
<attribute name="DESCRIPTION" value="Connector Header Through Hole 2 position 0.098&quot; (2.50mm) " constant="no"/>
<attribute name="MANUFACTURER" value="JST Sales America Inc. " constant="no"/>
<attribute name="MPN" value="B2B-XH-A(LF)(SN) " constant="no"/>
<attribute name="OPERATING_TEMPERATURE" value="-25°C ~ 85°C" constant="no"/>
<attribute name="PART_STATUS" value="ACTIVE" constant="no"/>
<attribute name="PITCH" value="0.098&quot; (2.50mm) " constant="no"/>
<attribute name="ROHS_COMPLIANCE" value="ROHS3 Compliant " constant="no"/>
<attribute name="SERIES" value="XH" constant="no"/>
<attribute name="SUB_CATEGORY" value="Rectangular Connectors " constant="no"/>
<attribute name="TYPE" value="Header, Male Pin, Board to Cable/Wire, Through Hole" constant="no"/>
</technology>
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
<part name="J1" library="Connector" deviceset="B4B-XH-A" device="" package3d_urn="urn:adsk.eagle:package:24957647/3"/>
<part name="J2" library="Connector" deviceset="B4B-XH-A" device="" package3d_urn="urn:adsk.eagle:package:24957647/3"/>
<part name="J3" library="Connector" deviceset="B2B-XH-A" device="" package3d_urn="urn:adsk.eagle:package:24957641/6"/>
<part name="J4" library="Connector" deviceset="B2B-XH-A" device="" package3d_urn="urn:adsk.eagle:package:24957641/6"/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="J1" gate="G$1" x="20.32" y="27.94" smashed="yes">
<attribute name="NAME" x="20.32" y="32.004" size="1.778" layer="95" align="bottom-center"/>
</instance>
<instance part="J2" gate="G$1" x="20.32" y="12.7" smashed="yes">
<attribute name="NAME" x="20.32" y="16.764" size="1.778" layer="95" align="bottom-center"/>
</instance>
<instance part="J3" gate="G$1" x="-2.54" y="43.18" smashed="yes" rot="R90">
<attribute name="NAME" x="-7.874" y="43.18" size="1.778" layer="95" rot="R90" align="bottom-center"/>
</instance>
<instance part="J4" gate="G$1" x="-2.54" y="-5.08" smashed="yes" rot="R270">
<attribute name="NAME" x="2.794" y="-5.08" size="1.778" layer="95" rot="R270" align="bottom-center"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="+12V" class="0">
<segment>
<wire x1="0" y1="0" x2="0" y2="10.16" width="0.1524" layer="91"/>
<wire x1="0" y1="10.16" x2="0" y2="25.4" width="0.1524" layer="91"/>
<label x="0" y="0.254" size="1.778" layer="95" rot="R90"/>
<pinref part="J4" gate="G$1" pin="1"/>
<pinref part="J2" gate="G$1" pin="3"/>
<wire x1="15.24" y1="10.16" x2="0" y2="10.16" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="3"/>
<wire x1="15.24" y1="25.4" x2="0" y2="25.4" width="0.1524" layer="91"/>
<label x="7.62" y="25.4" size="1.778" layer="95"/>
<label x="7.62" y="10.16" size="1.778" layer="95"/>
<junction x="0" y="10.16"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<wire x1="-2.54" y1="0" x2="-2.54" y2="7.62" width="0.1524" layer="91"/>
<wire x1="-2.54" y1="7.62" x2="-2.54" y2="22.86" width="0.1524" layer="91"/>
<label x="-2.54" y="0.508" size="1.778" layer="95" rot="R90"/>
<pinref part="J4" gate="G$1" pin="2"/>
<pinref part="J2" gate="G$1" pin="4"/>
<wire x1="15.24" y1="7.62" x2="-2.54" y2="7.62" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="4"/>
<wire x1="15.24" y1="22.86" x2="-2.54" y2="22.86" width="0.1524" layer="91"/>
<label x="7.62" y="22.86" size="1.778" layer="95"/>
<label x="7.62" y="7.62" size="1.778" layer="95"/>
<junction x="-2.54" y="7.62"/>
</segment>
</net>
<net name="TACH" class="0">
<segment>
<pinref part="J3" gate="G$1" pin="1"/>
<label x="-5.08" y="30.48" size="1.778" layer="95" rot="R90"/>
<pinref part="J2" gate="G$1" pin="2"/>
<wire x1="15.24" y1="12.7" x2="-5.08" y2="12.7" width="0.1524" layer="91"/>
<wire x1="-5.08" y1="12.7" x2="-5.08" y2="38.1" width="0.1524" layer="91"/>
<label x="7.62" y="12.7" size="1.778" layer="95"/>
</segment>
</net>
<net name="PWM" class="0">
<segment>
<pinref part="J2" gate="G$1" pin="1"/>
<wire x1="15.24" y1="15.24" x2="2.54" y2="15.24" width="0.1524" layer="91"/>
<wire x1="2.54" y1="15.24" x2="2.54" y2="30.48" width="0.1524" layer="91"/>
<wire x1="2.54" y1="30.48" x2="15.24" y2="30.48" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="1"/>
<wire x1="2.54" y1="30.48" x2="-2.54" y2="30.48" width="0.1524" layer="91"/>
<pinref part="J3" gate="G$1" pin="2"/>
<wire x1="-2.54" y1="30.48" x2="-2.54" y2="38.1" width="0.1524" layer="91"/>
<label x="7.62" y="15.24" size="1.778" layer="95"/>
<label x="-2.54" y="30.48" size="1.778" layer="95" rot="R90"/>
<label x="7.62" y="30.48" size="1.778" layer="95"/>
<junction x="2.54" y="30.48"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports the association of 3D packages
with devices in libraries, schematics, and board files. Those 3D
packages will not be understood (or retained) with this version.
</note>
</compatibility>
</eagle>
