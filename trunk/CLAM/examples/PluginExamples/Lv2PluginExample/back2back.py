#!/usr/bin/python
import sys, os
sys.path.append('../../../scripts/')
from audiob2b import runBack2BackProgram

network_test = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.1" id="Unnamed">

  <description>&lt;!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd"&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name="qrichtext" content="1" /&gt;&lt;style type="text/css"&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;"&gt;
&lt;p style="-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'DejaVu Sans'; font-size:11pt;"&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</description>

  <processing id="Input" position="151,180" size="128,108" type="AudioSource">
    <NSources>2</NSources>
  </processing>

  <processing id="Output" position="370,183" size="128,111" type="AudioSink">
    <NSinks>2</NSinks>
  </processing>

  <port_connection>
    <out>Input.1</out>
    <in>Output.1</in>
  </port_connection>

  <port_connection>
    <out>Input.2</out>
    <in>Output.2</in>
  </port_connection>

</network>

"""

if __name__ == '__main__':

	data_path="../../../../../clam-test-data/b2b/lv2_plugin/"

	f = open('test_othercable.clamnetwork','w')

	f.write(network_test)	

	f.close()

	back2BackTests = [
		("simple_ttl",
		"clam_lv2_generator.py --ttl -u %(uri)s %(network)s > %(target)s" 
			% dict(   uri="http://clam-project.org/examples/lv2"
				, network="test_othercable.clamnetwork"
				, target="output.ttl")
		, [
			"output.ttl",
		]),
	]

	runBack2BackProgram(data_path, sys.argv, back2BackTests)
	os.remove('test_othercable.clamnetwork')
