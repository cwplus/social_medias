<?php
function get_facebook($url,$short=false) {
 	$json_string = file_get_contents('https://api.facebook.com/method/links.getStats?format=json&urls=' . $url );
 	$json = json_decode($json_string, true);
 	if(isset($json[0]['total_count'])){
	 		return ($short)?shorterTotal(intval( $json[0]['total_count'] )):intval( $json[0]['total_count'] );
 	} else { return 0;}
}

function get_twitter($url,$short=false) {
    $json_string = file_get_contents('http://urls.api.twitter.com/1/urls/count.json?url=' . $url);
    $json = json_decode($json_string, true);
 	if(isset($json['count'])){
        return ($short)?shorterTotal(intval( $json['count'] )):intval( $json['count'] );
 	} else {return 0;}
}

function get_xing($url,$short=false){
// CURL initiieren & Parameter Setzen
    $ch = curl_init();
    curl_setopt($ch,CURLOPT_URL, 'https://www.xing-share.com/spi/shares/statistics');
    curl_setopt($ch,CURLOPT_POST, 1);
    curl_setopt($ch,CURLOPT_POSTFIELDS, 'url='.$url);
    curl_setopt($ch,CURLOPT_RETURNTRANSFER, true);

// CURL Resultate in String schreiben und schließen
    $curl_results = curl_exec($ch);
    curl_close($ch);

// JSON Parsen
    $json = json_decode($curl_results,true);

// Rückgabe vorbereiten
    if($json['share_counter'])
        return ($short)?shorterTotal(intval( $json['share_counter'] )):intval( $json['share_counter'] );
    return 0;
}

function get_google($url,$short=false)
{
    $json_string = file_get_contents('https://apis.google.com/u/0/se/0/_/+1/sharebutton?plusShare=true&url='.$url);
    preg_match( '/ld:\[[^,]+,\[\d+,(\d+),/', $json_string, $matches);
    if(isset($matches[1])){
        $count = (int) $matches[1];
        return ($short)?shorterTotal($count):$count;
    }
    return 0;
}

function get_delicious($url,$short=false)
{
    $json_string = file_get_contents('http://feeds.delicious.com/v2/json/urlinfo/data?url='.$url);
    $json = json_decode($json_string,true);
    if(isset($json[0])){
        $count = (int) $json[0]->total_posts;
        return ($short)?shorterTotal($count):$count;
    }
    return 0;
}

function get_reddit($url,$short=false)
{
    $json_string = file_get_contents('http://www.reddit.com/api/info.json?&url='.$url);
    $data = json_decode($json_string,true);
    $ups = 0;
    $downs = 0;
    foreach($data->data->children as $child) {
        $ups+= (int) $child->data->ups;
        $downs+= (int) $child->data->downs;
    }
    $count = $ups - $downs;
    return ($short)?shorterTotal($count):$count;
}

function get_buffer($url,$short=false)
{
    $json_string = file_get_contents('https://api.bufferapp.com/1/links/shares.json?url='.$url);
    $data = json_decode($json_string,true);
    $count = $data->shares;
    return ($short)?shorterTotal($count):$count;
}

function get_vk($url,$short=false)
{
    $json_string = file_get_contents('https://vk.com/share.php?act=count&index=1&url='.$url);
    preg_match('/^VK.Share.count\(\d+,\s+(\d+)\);$/i', $json_string, $matches);
    $count = $matches[1];
    return ($short)?shorterTotal($count):$count;
}
  
/*function get_google($url,$short=false) {
    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, "https://clients6.google.com/rpc");
    curl_setopt($curl, CURLOPT_POST, 1);
    curl_setopt($curl, CURLOPT_POSTFIELDS, '[{"method":"pos.plusones.get","id":"p","params":{"nolog":true,"id":"' . $url . '","source":"widget","userId":"@viewer","groupId":"@self"},"jsonrpc":"2.0","key":"p","apiVersion":"v1"}]');
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($curl, CURLOPT_HTTPHEADER, array('Content-type: application/json'));
    $curl_results = curl_exec ($curl);
    curl_close ($curl);
 
    $json = json_decode($curl_results, true);
 	if(isset($json[0]['result']['metadata']['globalCounts']['count'])){
        $ret = intval( $json[0]['result']['metadata']['globalCounts']['count'] );
 		return ($short)?shorterTotal($ret):$ret;
 	} else {return 0;}
}*/

function get_stumbleupon($url,$short=false) {
	$json_string = file_get_contents('http://www.stumbleupon.com/services/1.01/badge.getinfo?url='.$url);
	$json = json_decode($json_string, true);
	if (isset($json['result']['views'])) {
        $ret = intval($json['result']['views']);
		return ($short)?shorterTotal($ret):$ret;
	} else {return 0;}
}

function get_linkedin($url,$short=false) {
	$json_string = file_get_contents('https://www.linkedin.com/countserv/count/share?format=json&url=' . $url);
	$json = json_decode($json_string, true);
	if(isset($json['count'])){
		return ($short)?shorterTotal(intval( $json['count'] )):intval( $json['count'] );
	} else {return 0;}
}

function get_pinterest($url,$short = false) {
    $json_string = file_get_contents('http://api.pinterest.com/v1/urls/count.json?url=' . $url);
    $json_string = substr( $json_string, 13, -1);
    $json = json_decode($json_string, true);
    if(isset($json['count'])){
        return ($short)?shorterTotal(intval( $json['count'] )):intval( $json['count'] );
    } else {return 0;}
}

/*function formatNumberAbbreviation($number) {
    if ($number != 0 && preg_match('/B|M|K/i', $number)) {
        var_dump($number);
        switch (strtolower(substr($number, -1))) {
            case 'k':
                $number*=1000;
                break;
            case 'm':
                $number*=1000000;
                break;
            case 'b':
                $number*=1000000000;
                break;
        }
    }
    return $number;
}*/

function shorterTotal($num) {
	if ($num >= 1e9) {
		$num = "<b class='counter'>".number_format(($num / 1e9), 0, ".", ""). "</b> B";
    } else if ($num >= 1e6) {
		$num = "<b class='counter'>".number_format(($num / 1e6), 0, ".", "")."</b> M";
    } else if ($num >= 1e3) {
		$num = "<b class='counter'>".number_format(($num / 1e3), 0, ".", "")."</b> k";
    }
	return $num;
}
