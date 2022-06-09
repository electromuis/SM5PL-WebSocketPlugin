<template>
  <b-container id="app" style="padding-top: 30px;">
    <b-form-input v-model.trim="filter" placeholder="Search ..." style="margin-bottom: 30px"></b-form-input>

    <b-table striped hover :items="filter ? itemsFiltered : items" :fields="fields" >

      <template #cell(actions)="row">
        <b-button size="sm" @click="() => selectSong(row.item)">
          Select
        </b-button>
      </template>

    </b-table>
  </b-container>
</template>

<script>

import {client,sendRequest} from './client.js'

export default {
  watch: {
    filter(newVal) {
      this.itemsFiltered = this.items.filter((i) => {
        const searchKey = [
          i.artist.toLowerCase(),
          i.title.toLowerCase(),
          i.group.toLowerCase()
        ].join(' ')

        return searchKey.includes(newVal.toLowerCase())
      })
    }
  },
  name: 'App',
  
  data() {
    return {
      items: [],
      itemsFiltered: [],
      fields: [
        {key: 'group', label: 'Pack'},
        {key: 'title', label: 'Title'},
        {key: 'artist', label: 'Artist'},
        {key: 'charts', label: 'Charts'},
        {key: 'actions', label: 'Actions'}
      ],
      loading: true,
      filter: ''
    }
  },
  methods: {
    sendRequest,
    selectSong(song) {
      console.log("Select: ", song);
      this.sendRequest("SelectSong", song)
    },
    loadSongs() {
      let me = this;

      this
      .sendRequest('GetGroups', {})
      .then(groupResult => {
        
        Promise.all(groupResult.groups.map(group => {
          
          return me
          .sendRequest("GetSongsInGroup", {group})
          .then(songResult => {
            songResult.songs.forEach(song => {
              song.group = group
              
              song.charts = song.steps
                .filter((s) => s.type == "Dance_Single")
                .map((s) => s.meter)
                .sort(function(a, b){return a-b})
                .join(', ')

              me.items.push(song)
              console.log(song)
            });

          })

        })).then(() => {
          me.loading = false;
          console.log("Loading done");
        });

      })
    }
  },
  created() {
    let me = this;

    client.onopen = function() {
      console.log("Open");
      me.loadSongs()
    };
  }
}
</script>

<style>

</style>
