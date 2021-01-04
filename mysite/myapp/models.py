from django.db import models
from django.contrib.auth.models import User
from math import *

# Create your models here.
class PlayerModel(models.Model):
    player = models.CharField(max_length=30)
    score = models.IntegerField()

    def __str__(self):
        return self.player + ": " + str(self.score)

