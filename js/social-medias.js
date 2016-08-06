/**
 * Created by khalid on 26/07/16.
 */


(function ($) {
    var sm = Drupal.settings.sm;
    var totalShare = 0;
    $.fn.snPopup = function (options) {
        // Default params
        var defaults =
        {
            "url": null,
            "type": 'facebook',
            "title": null,
            "via": "ElhakymKhalid",
            "media": null,
            "lang": 'en-US',
            "count": null,
            "hashtags": '',
            "xing_follow": null,
            "hide_counter":null,
            "counters": Object.keys(sm.social_medias_has_counter).length,
            "ajax_counter_url": sm.ajax_counter_url,
        };
        var parametres = $.extend(defaults, options);

        function shorterTotal(num) {
            if (num >= 1e9) {
                num = "<b class='counter'>" + (num / 1e9).toFixed(0) + "</b> B"
            } else if (num >= 1e6) {
                num = "<b class='counter'>" + (num / 1e6).toFixed(0) + "</b> M"
            } else if (num >= 1e3) {
                num = "<b class='counter'>" + (num / 1e3).toFixed(0) + "</b> k"
            } else {
                num = "<b>"+num+"</b>";
            }
            return num;
        }

        //console.log($(".social-media"));
        this.each(function (i) {
            var span_total_counter = $(this).find('.total .s-counter');
            var node = $(this).data('node');
            var elemnt_id = $(this).attr('id');
            var $$this = $('#'+elemnt_id+' .social');
            var _i = 0;
            var totalShare = 0;
            $$this.each(function (j) {
                var $$$this = $(this);

                // If option on the link attributs
                var url = ($(this).attr('data-url') != undefined && $(this).attr('data-url') != '') ? $(this).attr('data-url') : parametres.url;
                var type = ($(this).attr('data-type') != undefined && $(this).attr('data-type') != '') ? $(this).attr('data-type') : parametres.type;
                var title = ($(this).attr('data-title') != undefined && $(this).attr('data-title') != '') ? $(this).attr('data-title') : parametres.title;
                var via = ($(this).attr('data-via') != undefined && $(this).attr('data-via') != '') ? $(this).attr('data-via') : parametres.via;
                var count = ($(this).attr('data-count') != undefined && $(this).attr('data-count') != '') ? $(this).attr('data-count') : parametres.count;
                count = parseInt(count);
                var ajax_counter_url = ($(this).attr('data-ajax-counter-url') != undefined && $(this).attr('data-ajax-counter-url') != '') ? $(this).attr('data-ajax-counter-url') : parametres.ajax_counter_url;
                var media = ($(this).attr('data-media') != undefined && $(this).attr('data-media') != '') ? $(this).attr('data-media') : parametres.media;
                var lang = ($(this).attr('data-lang') != undefined && $(this).attr('data-lang') != '') ? $(this).attr('data-lang') : parametres.lang;
                var hashtags = ($(this).attr('data-hashtags') != undefined && $(this).attr('data-hashtags') != '') ? $(this).attr('data-hashtags') : parametres.hashtags;
                var xing_follow = ($(this).attr('data-xing-follow') != undefined && $(this).attr('data-xing-follow') != '') ? $(this).attr('data-xing-follow') : parametres.xing_follow;
                var hide_counter = ($(this).attr('data-hide-counter') != undefined && $(this).attr('data-hide-counter') != '') ? $(this).attr('data-hide-counter') : parametres.hide_counter;
                var popupWidth = 0;
                var popupHeight = 0;
                var shareUrl = '';
                // Url use to share
                url = (url != null) ? url : document.URL;
                $URL = url;

                switch (type) {
                    case 'twitter': // Twitter share
                        var twitter = $(this);
                        popupWidth = 550;
                        popupHeight = 420;
                        shareUrl = (title != null) ? 'https://twitter.com/intent/tweet?original_referer=' + encodeURIComponent(document.URL) + '&text=' + encodeURIComponent(title) + '&url=' + encodeURIComponent(url)+'&hashtags='+encodeURIComponent(hashtags) : 'https://twitter.com/intent/tweet?original_referer=' + encodeURIComponent(document.URL) + '&url=' + encodeURIComponent(url)+'&hashtags='+encodeURIComponent(hashtags);
                        shareUrl = (via != null) ? shareUrl + '&via=' + via : shareUrl;
                        break;
                    case 'google': // Google + share

                        popupWidth = 600;
                        popupHeight = 600;
                        shareUrl = 'https://plus.google.com/share?url=' + url;
                        shareUrl = (lang != null) ? shareUrl + '&hl=' + lang : shareUrl;

                        break;
                    case 'xing': // Xing
                        popupWidth = 600;
                        popupHeight = 400;
                        shareUrl = 'https://www.xing.com/spi/shares/new?url=' + encodeURIComponent(url);
                        if(xing_follow != null) shareUrl += '&follow_url='+ encodeURIComponent(xing_follow);
                        shareUrl = (lang != null) ? shareUrl + '&lang=' + lang : shareUrl;
                        break;
                    case 'linkedin' : // Linkedin share
                        var linkedin = $(this);
                        popupWidth = 600;
                        popupHeight = 213;
                        shareUrl = 'https://www.linkedin.com/shareArticle?url=' + url;
                        break;
                    case 'stumbleupon': // StumbleUpon share
                        var stumbleupon = $(this);
                        popupWidth = 1000;
                        popupHeight = 617;
                        shareUrl = 'http://www.stumbleupon.com/submiturl=' + url;
                        shareUrl = (title != null) ? shareUrl + '&title=' + title : shareUrl;
                        break;
                    case 'pinterest': // Pinterest share
                        var pinterest = $(this);
                        popupWidth = 1000;
                        popupHeight = 617;
                        shareUrl = 'http://www.pinterest.com/pin/create/button/?url=' + url;
                        shareUrl = (media != null) ? shareUrl + '&media=' + media : shareUrl;
                        shareUrl = (title != null) ? shareUrl + '&description=' + title : shareUrl;
                        break;
                    case 'delicious': // Pinterest share
                        var delicious = $(this);
                        popupWidth = 550;
                        popupHeight = 550;
                        shareUrl = 'http://www.delicious.com/save?v=5&noui&jump=close&url=' + encodeURIComponent(url);
                        shareUrl = (title != null) ? shareUrl + '&title=' + encodeURIComponent(title) : shareUrl;
                        break;
                    case 'reddit': // Pinterest share
                        var reddit = $(this);
                        popupWidth = 550;
                        popupHeight = 550;
                        shareUrl = 'https://reddit.com/submit?url=' + encodeURIComponent(url);
                        shareUrl = (title != null) ? shareUrl + '&title=' + encodeURIComponent(title) : shareUrl;
                        break;
                    case 'vk': // Pinterest share
                        var vk = $(this);
                        popupWidth = 550;
                        popupHeight = 550;
                        shareUrl = 'http://vk.com/share.php?url=' + encodeURIComponent(url);
                        shareUrl = (media != null) ? shareUrl + '&image=' + media : shareUrl;
                        shareUrl = (title != null) ? shareUrl + '&title=' + encodeURIComponent(title) : shareUrl;
                        break;
                    default: // Default Facebook share
                        var facebook = $(this);
                        popupWidth = 670;
                        popupHeight = 340;
                        shareUrl = 'https://www.facebook.com/sharer/sharer.php?u=' + url;
                        break;
                }
                if (count) {
                    _i++;
                    $.getJSON(ajax_counter_url + '?url=' + $URL + '&service='+type+'&node='+node).success(function (resultdata) {
                        var counter = (isNaN(parseInt(resultdata.count))) ? 0 : parseInt(resultdata.count);
                        //counter = Math.floor((Math.random() * 1000000) + 1);
                        totalShare += counter;
                        if(hide_counter) $$$this.find('.counter').html(shorterTotal(counter));
                        //totalShare += Math.floor((Math.random() * 1000000) + 1);
                        //console.log('node: %d, total: %d, _i: %d, counters: %d',node,totalShare,_i,parametres.counters);
                        if(_i == parametres.counters) {
                            span_total_counter.html(shorterTotal(totalShare));
                        }
                        //console.log(type+'==>'+totalShare);
                    });

                }

                // Click to Action Open Popup Share
                $(this).on('click tap', function () {
                    // Center popup to the screen
                    var left = (screen.width / 2) - (popupWidth / 2);
                    var top = (screen.height / 2) - (popupHeight / 2);
                    popupWindow = window.open(shareUrl, 'popUpWindow', 'height=' + popupHeight + ',width=' + popupWidth + ',left=' + left + ',top=' + top + ',resizable=yes,scrollbars=yes,toolbar=no,menubar=no,location=no,directories=no,status=yes');
                    return false;
                });
            });

        });
    };
})(jQuery);

jQuery(".social-media").snPopup();