var app = new Vue({
    el: '#printplayers',
    data: {
        player: [],
        seen:true,
        unseen:false
    },

    computed: {
        sortedPlayers: function() {
            function compare(a, b) {
                if (a.score < b.score)
                    return 1;
                if (a.score > b.score)
                    return -1;
                return 0;
            }
            return this.player.sort(compare);
        }
    },

      //Adapted from https://stackoverflow.com/questions/36572540/vue-js-auto-reload-refresh-data-with-timer
    created: function() {
        this.fetchPlayerList();
        this.timer = setInterval(this.fetchPlayerList, 2500);
    },

    methods: {
        fetchPlayerList: function() {
            axios.get('/players/').then(
                response => (this.player = response.data.players))
            this.seen = false
            this.unseen = true
        },
        cancelAutoUpdate: function() { clearInterval(this.timer) }
    },

    beforeDestroy() {
        this.cancelAutoUpdate();
    }
});