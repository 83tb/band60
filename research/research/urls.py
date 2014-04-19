from django.conf.urls import patterns, include, url

from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'research.views.home', name='home'),
    url(r'^r/', include('quickstart.urls')),

    url(r'^admin/', include(admin.site.urls)),
)
