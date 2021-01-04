from django import forms
from django.core.validators import validate_slug
from . import models

class PlayerForm(forms.Form):
    player = forms.CharField(
        required=True,
        max_length=30,
    )
    score = forms.IntegerField(
        required=True,
        min_value=0,
    )

    def save(self, request):
        player_instance = models.PlayerModel()
        player_instance.player = self.cleaned_data["player"]
        player_instance.score = self.cleaned_data["score"]
        player_instance.save()
        return player_instance