from django.shortcuts import render
from django.http import JsonResponse

from . import models
from . import forms

def get_players(request):
    players = models.PlayerModel.objects.all()
    player_list = {}
    player_list["players"] = []
    for stor in players:
        temp_player = {}
        temp_player["player"] = stor.player
        temp_player["score"] = stor.score
        player_list["players"] += [temp_player]
    return JsonResponse(player_list)
        

def index(request):
    if request.method == "POST":
        player_form = forms.PlayerForm(request.POST)
        if player_form.is_valid():
            player_form.save(request)
            player_form = forms.PlayerForm()
    else:
        player_form = forms.PlayerForm(request.POST)
    displayMsg = "BLOCK BREAKER"
    player = models.PlayerModel.objects.all()
    context = {
        "displayMsg": displayMsg,
        "player": player,
        "form": player_form,
    }
    return render(request, 'index.html', context=context)
