from django.conf.urls import patterns, url, include
from rest_framework import routers
from quickstart import views

from rest_framework.urlpatterns import format_suffix_patterns

from quickstart.views import TimeSeriesView


router = routers.DefaultRouter()
router.register(r'users', views.UserViewSet)
router.register(r'groups', views.GroupViewSet)
router.register(r'people', views.PersonViewSet)
router.register(r'timeseries', views.TimeSeriesViewSet)



# Wire up our API using automatic URL routing.
# Additionally, we include login URLs for the browseable API.
urlpatterns = patterns('',
    
    url(r'^data', TimeSeriesView.as_view()),
    
   

)


urlpatterns = format_suffix_patterns(urlpatterns, suffix_required=True)


urlpatterns += patterns('',
    url(r'^', include(router.urls)),
   
    
    url(r'^api-auth/', include('rest_framework.urls', namespace='rest_framework'))

)
