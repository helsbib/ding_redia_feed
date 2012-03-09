# Ding! Redia feeds

`ding_redia_feeds` is a module which exposes content on a Ding! 1.x site to a mobile app developed by [Redia](http://www.redia.dk/) through a number of RSS feeds.

The primary use case for this is article and event nodes provided by the [`ding_content` module](https://github.com/dingproject/ding/tree/master/ding_content).

## Configuration

The module can be configured in the administation interface at `admin/settings/ding_redia_feed`.

![Redia feed settings](https://github.com/downloads/helsbib/ding_redia_feed/redia_feed_configuration.png)

Here you can configure which content types should support the Redia app category and what categories are available. 

Currently the categories are used to specify which content should be highlighted within the app.

Typically `articles` (*Blogindlæg*) use the category.

###Important notes regarding categories

Categories differ between apps and the ids and descriptions for your app must be obtained from Redia. The provided defaults are merely examples.

If changing id for a category all existing content using the category must be reassigned to the category.

## Use

When installed the module exposes a number of feeds which can be used by the Redia app:

* `redia/feed`: All recent articles
* `redia/arrangementer/feed`: All upcoming events
* `ding_redia_feed/mobileapp.xml`: Content specifically flagged for display in the app

The URLs for these feeds must be provided to Redia.

## Credits

The initial version of these feeds have been developed for [Copenhagen Public Libraries](http://bibliotek.kk.dk). They have since been generalized for use by more libraries and applications by [Elsinore Public Libraries](http://helsbib.dk).