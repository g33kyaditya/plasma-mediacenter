
var panel = new Panel
panel.screen = 0
panel.location = 'top'
panel.addWidget("org.kde.plasma.kickoff")
panel.addWidget("org.kde.plasma.pager")
panel.addWidget("org.kde.plasma.taskmanager")
panel.addWidget("org.kde.plasma.systemtray")
panel.addWidget("org.kde.plasma.digitalclock")

for (var i = 0; i < screenCount; ++i) {
    var id = createActivity("Plasma mediacenter");
    var desktopsArray = desktopsForActivity(id);
    print(desktopsArray.length);
    for( var j = 0; j < desktopsArray.length; j++) {
        desktopsArray[j].wallpaperPlugin = "org.kde.color";
        print(desktopsArray[j].wallpaperPlugin);
        //var clock = desktopsArray[j].addWidget("org.kde.plasma.analogclock");
    }
}